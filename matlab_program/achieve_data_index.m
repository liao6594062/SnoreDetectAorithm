function  index_data=achieve_data_index(fft_len,tree_deep)

y=1:fft_len;

if log2(fft_len)<=tree_deep
    tree_deep=log2(fft_len);
end

for l=1:tree_deep
    step=2^l;
    step_font=2^(l-1);
    
    for j=1:2^(l-1)
        for  k=1:fft_len/step
           x11=y((j-1)*fft_len/step_font+1)+(k-1)*step;
           x12=y((j-1)*fft_len/step_font+2)+(k-1)*step;
           y((j-1)*fft_len/step_font+k)=x11;
           y((j-1)*fft_len/step_font+k+fft_len/step)=x12;
        end
    end
end

index_data=y;
        
        
        
        

        
        
        
        
       


 

        

