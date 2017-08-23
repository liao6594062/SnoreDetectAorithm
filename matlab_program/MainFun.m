[audio_data,frequent_sample]=audioread('≤‚ ‘.m4a');
audio_data=audio_data(1:6:end);

window_data=[];
is_snore_vector=[];
snore_feature_data_vector=[];
window_size=128;
precise_sample=128;
max_range_throld_value=0.79;

for i=1:size(audio_data,1)
    window_data=window_data_struct(window_data,window_size,audio_data(i));
    if  size(window_data,1)==window_size&&mod(i-window_size,precise_sample)==0
         snore_feature_data=achieve_snore_feature(window_data);
         is_snore=snore_judge_funs(snore_feature_data,max_range_throld_value);
         snore_feature_data_vector=[snore_feature_data_vector;snore_feature_data];
         is_snore_vector=[is_snore_vector;is_snore];
    end
end

figure,plot(audio_data)
figure,plot(is_snore_vector);ylim([0,5])
        
    
 
 
 
 