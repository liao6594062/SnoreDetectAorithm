function  snore_feature_data=achieve_snore_feature(window_data)

snore_feature_data=zeros(1,2);
snore_feature_data(1,1)=max(window_data);
snore_feature_data(1,2)=max(window_data)-min(window_data);

