function   amplifydiff=amplifydiff_comput(data_vector)

amplifydiff=zeros(1,size(data_vector,2));
for  i=0:(size(data_vector,2)-1)
    for  j=1:(size(data_vector,2)-i)
        amplifydiff(1,i+1)=amplifydiff(1,i+1)+abs(data_vector(i+j)-data_vector(j));
    end
end
    