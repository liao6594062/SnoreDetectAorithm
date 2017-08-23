function  fft_value=comput_fft(data,fft_len,k)

if  log2(fft_len)~=fix(log2(fft_len))
    error('fft_len必须是2^k,k为整数');
end

index_data=achieve_data_index(fft_len,k);

y_fftn=data(1,index_data);
for l=(log2(fft_len)-1):-1:0
     class_num=2^l;
     every_class_num=fft_len/class_num;
     for  j=1:class_num
         for k=1:every_class_num/2
             temp1=y_fftn((j-1)*every_class_num+k);
             temp2=y_fftn((j-1)*every_class_num+k+every_class_num/2);
             y_fftn((j-1)*every_class_num+k)=temp1+exp(-2*pi*(k-1)*i/every_class_num)*temp2;
             y_fftn((j-1)*every_class_num+k+every_class_num/2)=temp1-exp(-2*pi*(k-1)*i/every_class_num)*temp2;
         end
     end
end

fft_value=y_fftn;
         
         
         
         
    
    


    













    
    
        
    
    
    
  
       
       
       
           
        
        
        
        
    



