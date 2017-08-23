function  mfcc_feature = mfcc_feature_achieve(window_data,nfft,bank,K)

w=1+0.5*K*sin(pi*[1:K]./K);%归一化倒谱提升窗口  
w=w/max(w);%预加重滤波器
 y=window_data*(1-0.9375);  %语音分帧
 s=y'.*hamming(nfft);  
 t=abs(fft(s));%FFT快速傅里叶变换  
 t=t.^2; 
 for k=1:K  
    n=0:(size(bank,1)-1);  
    dctcoef(k,:)=cos((2*n+1)*k*pi/(2*24));  %计算dct系数
end  
 c1=dctcoef*log(bank*t(1:size(bank,2))+1);  
 mfcc_feature=c1.*w';  %获取mfcc
 
  
