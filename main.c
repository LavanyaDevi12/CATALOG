#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <cjson/cJSON.h>

typedef long long ll;

ll baseToDecimal(const char *value, int base) {
    ll result = 0;
    for (int i = 0; value[i]; i++) {
        int digit;
        if (isdigit(value[i])) digit = value[i] - '0';
        else digit = tolower(value[i]) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

long double lagrangeInterpolation(ll x[], ll y[], int k) {
    long double result = 0.0;

    for (int i = 0; i < k; i++) {
        long double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - x[j]);
                term /= (x[i] - x[j]);
            }
        }
        result += term;
    }

    return result;
}

void processFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    rewind(fp);

    char *data = malloc(length + 1);
    fread(data, 1, length, fp);
    data[length] = '\0';
    fclose(fp);

    cJSON *root = cJSON_Parse(data);
    if (!root) {
        printf("Error parsing JSON\n");
        free(data);
        return;
    }

    cJSON *keys = cJSON_GetObjectItem(root, "keys");
    int n = cJSON_GetObjectItem(keys, "n")->valueint;
    int k = cJSON_GetObjectItem(keys, "k")->valueint;

    ll x[20], y[20];
    int count = 0;

    cJSON *child = root->child;
    while (child) {
        if (strcmp(child->string, "keys") != 0) {
            ll xi = atoll(child->string);
            int base = atoi(cJSON_GetObjectItem(child, "base")->valuestring);
            const char *val = cJSON_GetObjectItem(child, "value")->valuestring;
            ll yi = baseToDecimal(val, base);
            x[count] = xi;
            y[count] = yi;
            count++;
        }
        child = child->next;
    }

    long double secret = lagrangeInterpolation(x, y, k);
    printf("Secret from %s: %.0Lf\n", filename, secret);

    cJSON_Delete(root);
    free(data);
}

int main() {
    processFile("testcase1.json");
    processFile("testcase2.json");
    return 0;
}
