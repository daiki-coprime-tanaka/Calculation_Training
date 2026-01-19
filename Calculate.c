#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define abs(x) (x) < 0 ? (-(x)) : ((x))

typedef struct questionanswer{
    int answer;
    char question[100];
    char* que;
    int qd;
} QA;

double clk(struct timeval tv) {
    return (double) (tv.tv_sec + (double) tv.tv_usec / 1000000.0);
}

int digits(int decimal) {
    int tem = decimal, ans = 0;
    if(decimal == 0) {
        ans++;
    }
    while(tem != 0) {
        tem /= 10;
        ans++;
    }
    return ans;
}

char* cw(int tf) {
    if(tf == 1) {
        return "〇";
    } else if(tf == 0) {
        return "×";
    } else {
        return "";
    }
}

QA prepareAnswer(int calkind, int a, int b, int dif) {
    QA ret;
    ret.answer = 0;
    for(int roop = 0; roop < 100; roop++) {
        ret.question[roop] = ' ';
    }
    if(calkind > 2) {
        int c = rand() % (dif * dif);
        a = c;
        b++;
    } else if (calkind < 2) {
        int c = rand() % (dif * dif);
        int d = rand() % (dif * dif);
        a = c;
        b = d;
    }
    int da = digits(a);
    int db = digits(b);
    ret.qd = da + db + 3;
    ret.que = malloc(sizeof(char) * ret.qd);
    int nowp = da - 1, temp = a;
    while(nowp >= 0) {
        ret.question[nowp] = '0' + temp % 10;
        temp /= 10;
        nowp--;
    }
    nowp = da + db + 2, temp = b;
    while(nowp > da + 2) {
        ret.question[nowp] = '0' + temp % 10;
        temp /= 10;
        nowp--;
    }
    switch(calkind) {
        case 0 :
            ret.answer = a + b;
            ret.question[da + 1] = '+';
            break;
        case 1 :
            ret.answer = a - b;
            ret.question[da + 1] = '-';
            break;
        case 2 :
            ret.answer = a * b;
            ret.question[da + 1] = '*';
            break;
        case 3 :
            ret.answer = a / b;
            ret.question[da + 1] = '/';
            break;
        case 4 :
            ret.answer = a % b;
            ret.question[da + 1] = '%';
            break;
    }
    strncpy(ret.que, ret.question, ret.qd);
    return ret;
}

int main(int argc, char* argv[]) {

    // 入力から難易度と問題数を決めています。
    int diff, qty;
    if(argc != 3) {
        printf("You should input \n%s unsigned int difficulty unsigned int quantity \nat command line.\n", argv[0]);
        return -1;
    } else {
        diff = abs(atoi(argv[1]));
        qty = abs(atoi(argv[2]));
        if(diff == 0) {
            diff++;
        }
        if(qty == 0) {
            qty++;
        }
    }

    // 何回目の実行かを計算し、その回数に対応するファイルを作ります。
    FILE *fp, *fq, *fr;
    fp = fopen("CState.txt", "r+");
    if(fp == NULL) {
        printf("Sorry, we failed to open \'CState.txt\'\n");
        return -2;
    }
    int count = 0;
    fscanf(fp, "%d", &count);
    count = count + 1;
    fp = fopen("CState.txt", "w+");
    if(fp == NULL) {
        printf("Sorry, we failed to open \'CState.txt\'\n");
        return -3;
    }
    fprintf(fp, "%d\n", count);
    fclose(fp);
    int digit = digits(count);
    char *num;
    char *name = malloc(sizeof(char) * (digit + 10));
    char *fname = malloc(sizeof(char) * (digit + 11));
    if(name == NULL || fname == NULL) {
        printf("Sorry, we failed to prepare enough space to store data of your input.\n");
        return -4;
    }
    name[0] = 'N', name[1] = 'o', name[2] = '.';
    fname[0] = 'N', fname[1] = 'o', fname[2] = '.';
    int tem = count, place = digit + 2;
    while(tem != 0) {
        name[place] = '0' + (tem % 10);
        fname[place] = '0' + (tem % 10);
        tem /= 10;
        place--;
    }
    name[digit + 3] = '-',  name[digit + 4] = 'c', name[digit + 5] = '.';
    fname[digit + 3] = '-', fname[digit + 4] = 'c', fname[digit + 5] = 'f', fname[digit + 6] = '.';
    name[digit + 6] = name[digit + 8] = 't', name[digit + 7] = 'x';
    fname[digit + 7] = fname[digit + 9] = 't', fname[digit + 8] = 'x';
    printf("Your result will be written in : %s, %s\n", name, fname);
    fq = fopen(name, "w+");
    fr = fopen(fname, "w+");
    if(fq == NULL || fr == NULL) {
        printf("Sorry, we failed to open \'%s\'\n", name);
        return -5;
    }

    // さあ、いよいよあなたが計算を行います
    srand(time(NULL));
    int point = 0;
    struct timeval start;
    gettimeofday(&start, NULL);
    double st = clk(start);
    double now = 0.0, prev = 0.0;
    prev = st;
    int quantity = qty;
    int difficulty = diff;
    int qdig = digits(quantity);
    int ddig = digits(difficulty);
    fprintf(fr, "問題番号|問題|解答|正解|正誤|解答時間(秒)\n");
    for(int i = 0; i < quantity; i++) {
        int a = rand() % difficulty, b = rand() % difficulty, c = rand() % 5;
        QA data = prepareAnswer(c, a, b, difficulty);
        int tf = 0;
        printf("(%d) %s = ", i + 1, data.que);
        int dans = -1;
        scanf("%d", &dans);
        if(dans == data.answer) {
            point++;
            tf = 1;
        }
        struct timeval wt;
        gettimeofday(&wt, NULL);
        now = clk(wt);
        fprintf(fr, "%*d|%s =|%*d|%*d|%s|%f\n", qdig, i + 1, data.que, digits(dans), dans, digits(data.answer), data.answer, cw(tf), now - prev);
        fprintf(fq, "(%*d) %s = %*d %*d %s %f\n", qdig, i + 1, data.que, digits(dans), dans, digits(data.answer), data.answer, cw(tf), now - prev);
        prev = now;
    }
    struct timeval end;
    gettimeofday(&end, NULL);
    double et = clk(end);
    double time = et - st;
    printf("所要時間: \x1b[31m%lf\x1b[0m sec 得点 : \x1b[32m%d\x1b[0m / %d\n", time, point, qty);
    fprintf(fq, "所要時間: %lf sec 得点 : %d / %d\n", time, point, qty);
    fprintf(fq, "難易度 : %d, 問題数 : %d\n", diff, qty);
    fprintf(fr, "|所要時間|%lf|sec|得点|%d|\n", time, point);
    fprintf(fr, "|難易度|%d||問題数|%d|\n", diff, qty);
    fclose(fq);
    fclose(fr);
}