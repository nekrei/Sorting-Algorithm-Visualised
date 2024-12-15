#include<stdio.h>

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
void high(int lpos, int  rpos){
    printf("h %d %d\n", lpos, rpos);
}

int main(){
    int n,i,j,min;
    scanf("%d",&n);
    for(i=0; i<n; i++)
        scanf("%d",&a[i]);
    for(i=0; i<n; i++){
        for(j=0; j<(n-i-1); j++){
            high(0,n-i-1);
            min = comp(j,j+1,"<");
            swap(min,j);
        }
    }
    printf("z\n");
    return 0;
}