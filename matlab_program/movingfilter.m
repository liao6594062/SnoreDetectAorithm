function  AfterFilter_data=movingfilter(data_vector,filter_len)
%一次滑动滤波
AfterFilter_data=zeros(size(data_vector));
AfterFilter_data(1:(filter_len-1))=data_vector(1:(filter_len-1));
index_matrix=zeros(length(data_vector)-filter_len+1,filter_len);
for i=1:filter_len
    index_matrix(:,i)=data_vector(i:(length(data_vector)-filter_len+i)); %滑动滤波
end
AfterFilter_data(filter_len:end)=mean(index_matrix,2);
% for i=filter_len:length(data)
%     AfterFilter_data(i)=mean(data((i-filter_len+1):i));
% end
    





