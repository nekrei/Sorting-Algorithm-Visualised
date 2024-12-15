#include<stdio.h>
#include<stdlib.h>

int a[10000];
int comp(int lpos, int rpos, char s[]){
    int val = s[0]+s[1];
    int m,n;
    switch (val)
    {
    case ('>'):
        m = (a[lpos]>a[rpos])? lpos : rpos;
        n = (a[lpos]>a[rpos])? rpos : lpos;
        break;
    case ('<'):
        m = (a[lpos]<a[rpos])? lpos : rpos;
        n = (a[lpos]<a[rpos])? rpos : lpos;
        break;
    case ('>'+'='):
        m = (a[lpos]>=a[rpos])? lpos : rpos;
        n = (a[lpos]>=a[rpos])? rpos : lpos;
        break;
    case ('<'+'='):
        m = (a[lpos]<=a[rpos])? lpos : rpos;
        n = (a[lpos]<=a[rpos])? rpos : lpos;
        break;
    default:
        return -1;
    }
    printf("c %d %d\n",m, n);
    return m;
}
void swap(int lpos, int rpos){
    if(lpos == rpos) return;
    int t = a[lpos];
    a[lpos] = a[rpos];
    a[rpos] = t;
    printf("s %d %d\n",lpos,rpos);
}

void high(int lpos, int  rpos, char op){
    printf("%c %d %d\n",op, lpos, rpos);
}


void qSort(int begin, int end){
    if(begin<end){
        high(begin,end, 'h');
        int pivot = begin;
        int pval = a[end];
        for(int i=begin; i<= end; i++){
                printf("p %d\n",pivot);
            comp(i,end,"<");
            if(a[i]< pval){
                swap(i,pivot);
                pivot++; 
            }
        }
        swap(end,pivot);
        qSort(begin, pivot-1);
        high(begin,end, 'r');
        qSort(pivot+1, end);
        high(begin,end, 'r');
    }
}

int main(){
    int n;
    scanf("%d",&n);
    for(int i=0; i<n; i++) scanf("%d",&a[i]);
    qSort(0,n-1);
    printf("z\n");
    return 0;
}