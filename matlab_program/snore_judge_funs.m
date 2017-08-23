function   is_snore=snore_judge_funs(snore_feature_data,max_range_throld_value)

if  snore_feature_data(1,2)>=max_range_throld_value
    is_snore=1;
else
    is_snore=0;
end

