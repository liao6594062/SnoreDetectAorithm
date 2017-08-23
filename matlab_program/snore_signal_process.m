%% 原始信号读取
[audio_data,sample_frequent]=audioread('4460.wav','native');
audio_data=audio_data(:,1);
% audio_data=double(audio_data)/32767; %值落在[-1,1]之间,归一化处理

%% 信号分帧处理
 window_size=256;
 precise_sample_fs=128;
 index_matrix=enframe(audio_data,window_size,precise_sample_fs);
 index_matrix=index_matrix.';
 movingfilter_data=data_movingfilter(index_matrix,5,5);  %滑动平均滤波
 index_matrix=movingfilter_data.';

% bpFilt = designfilt('bandpassfir', 'FilterOrder', 10, ...
%              'CutoffFrequency1', 10, 'CutoffFrequency2', 4000,... %带通滤波器
%              'SampleRate', sample_frequent);
% audio_data=filter(bpFilt,audio_data);
 
 %% 时域特征分析
 short_time_energy=sum(index_matrix.^2,2); %短时能量
 short_time_energy_db=max(10*log10(short_time_energy/window_size),0); %短时平均能量分贝数
 short_time_amplify=sum(abs(index_matrix),2);%短时平均幅度
 short_time_passZero=ceil(sum(abs(diff(sign(index_matrix),[],2)),2)/2);%短时过零率
 short_time_xcorr=zeros(size(index_matrix,1),2*size(index_matrix,2)-1);
 for i=1:size(index_matrix,1)                            %短时自相关函数
     short_time_xcorr(i,:)=xcorr(index_matrix(i,:));
 end
 short_time_amplifydiff=zeros(size(index_matrix));
 for i=1:size(index_matrix,1)                                        %短时平均幅度差
     short_time_amplifydiff(i,:)=amplifydiff_comput(index_matrix(i,:));
 end

 %% 频域特征分析
 index_matrix_fft=fft(index_matrix,window_size,2); %fft变换
 short_time_energy_spectrum=abs(index_matrix_fft).^2; %能量谱
 short_time_amplify_spectrum=abs(index_matrix_fft); %幅度谱
 short_time_angle_spectrum=angle(index_matrix_fft); %相位谱
 short_time_power_spectrum_density=abs(index_matrix_fft).^2/window_size;%功率谱
 short_time_sum_energy_spectrum=sum(short_time_energy_spectrum,2);%总能量谱（音量）
 short_time_energy_spectrum_db=max(10*log10(short_time_sum_energy_spectrum/window_size),0);%平均能量谱（音量分贝）
 short_time_mfcc_feature=zeros(size(index_matrix,1),12);
 for i=1:size(index_matrix,1)                                                                             %mfcc特征
   short_time_mfcc_feature(i,:)=mfcc_feature_achieve(index_matrix(i,:).',24,256,16000,0,0.5,12);
 end
 
 %% 绘图区
 figure('name','原始信号图'),plot(audio_data); %画原始信号时域波形图
 figure('name','短时能量'),plot(short_time_energy); %画短时能量图
 figure('name','短时平均能量分贝数'),plot(short_time_energy_db); %画短时平均能量分贝数图
 figure('name','短时平均幅度'),plot(short_time_amplify); %画短时平均幅度图
 figure('name','短时过零率'),plot(short_time_passZero); %画短时过零率图
%  figure('name','短时自相关函数'),plot(short_time_xcorr); %画短时自相关函数图
%  figure('name','短时平均幅度差函数'),plot(short_time_amplifydiff); %画短时平均幅度差函数图
%  figure('name','能量谱'),plot(short_time_energy_spectrum); %画能量谱图
%  figure('name','幅度谱'),plot(short_time_amplify_spectrum); %画幅度谱图
%  figure('name','相位谱'),plot(short_time_angle_spectrum);   %画相位谱图
%  figure('name','功率谱'),plot(short_time_power_spectrum_density); %画功率谱图
 figure('name','总能量谱'),plot(short_time_sum_energy_spectrum); %画总能量谱图
 figure('name','总能量谱（分贝值）'),plot(short_time_energy_spectrum_db); %画总能量谱（分贝值）图
%  figure('name','mfcc倒频谱'),imagesc(short_time_mfcc_feature); colorbar%画mfcc倒频谱图
 n=0:(window_size-1);
 fs=n*sample_frequent/window_size;
 figure('name','频谱图'),plot(fs(2:window_size/2),short_time_amplify_spectrum(928,2:window_size/2))%画频谱图