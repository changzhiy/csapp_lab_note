# Performance Lab

## Rotate Matrix

我个人想到比较直观的办法就是循环展开，上网查了一下好像没什么人做这个实验，也都差不多十几二十分，那就这样提交了。

```
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
    // 循环展开
    for (int i = 0; i < dim; i += 8) {
        for (int j = 0; j < dim; j += 8) {
            dst[RIDX(dim-1-j,i,dim)] = src[RIDX(i,j,dim)];
            dst[RIDX(dim-2-j,i,dim)] = src[RIDX(i,j+1,dim)];
            dst[RIDX(dim-3-j,i,dim)] = src[RIDX(i,j+2,dim)];
            dst[RIDX(dim-4-j,i,dim)] = src[RIDX(i,j+3,dim)];
            dst[RIDX(dim-5-j,i,dim)] = src[RIDX(i,j+4,dim)];
            dst[RIDX(dim-6-j,i,dim)] = src[RIDX(i,j+5,dim)];
            dst[RIDX(dim-7-j,i,dim)] = src[RIDX(i,j+6,dim)];
            dst[RIDX(dim-8-j,i,dim)] = src[RIDX(i,j+7,dim)];
        }
    }

}
```

## smooth 
这个smooth不用做得太复杂，比如去修改加法次数，这个该起来太麻烦了，实际上就把循环里的min, max减少就可以了，把矩形边缘的数据通过手动算法进行修改就可以实现效率的提升。 