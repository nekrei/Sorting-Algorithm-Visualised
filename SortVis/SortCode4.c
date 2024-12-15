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

void mSort(int begin, int end){
    if(begin<end){

            high(begin,end,'h');

        int m = (end+begin)/2;
        mSort(begin,m);

            high(begin,end,'r');

        mSort(m+1, end);

            high(begin,end,'r');

        int p[end-begin+1];
            printf("b %d %d\n", (end-begin+1), begin);
        int i=begin,j=m+1, pos = 0;
        while(i<=m || j<=end){
            if(i>m){
                    printf("c %d %d\n",j,j);
                p[pos] = a[j];
                    printf("i %d %d\n", pos, a[j]);
                j++;
            } 
            else if(j>end){
                    printf("c %d %d\n",i,i);
                p[pos] = a[i]; 
                    printf("i %d %d\n", pos, a[i]);
                i++;
            } else{
                int min = comp(i,j,"<");
                p[pos] = a[min];
                    printf("i %d %d\n", pos, a[min]);
                if(min==i) i++;
                else j++;
            }
            pos++;
        }
        printf("d %d\n",begin);
        for(i=begin; i<=end; i++) 
            a[i] = p[i-begin];
    }
}

int main(){
    int n;
    scanf("%d",&n);
    for(int i=0; i<n; i++) scanf("%d",&a[i]);
    mSort(0,n-1);
    printf("z\n");
    return 0;
}