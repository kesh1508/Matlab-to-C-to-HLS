
srate = 1000; 
time  = 0:1/srate:3;
n     = length(time);
p     = 15; 
noiseamp = 5; 
ampl   = interp1(rand(p,1)*30,linspace(1,p,n));
noise  = noiseamp * randn(size(time));
signal = ampl + noise;
fwhm = 25; 
k = 100;
gtime = 1000*(-k:k)/srate;
gauswin = exp( -(4*log(2)*gtime.^2) / fwhm^2 );
pstPeakHalf = k+dsearchn(gauswin(k+1:end)',.5);
prePeakHalf = dsearchn(gauswin(1:k)',.5);
empFWHM = gtime(pstPeakHalf) - gtime(prePeakHalf);
figure(1), clf, hold on
plot(gtime,gauswin,'ko-','markerfacecolor','w','linew',2)
plot(gtime([prePeakHalf pstPeakHalf]),gauswin([prePeakHalf pstPeakHalf]),'m','linew',3)
gauswin = gauswin / sum(gauswin);
title([ 'Gaussian kernel with requeted FWHM ' num2str(fwhm) ' ms (' num2str(empFWHM) ' ms achieved)' ])
xlabel('Time (ms)'), ylabel('Gain')
filtsigG = signal;
for i=k+1:n-k-1
    filtsigG(i) = sum( signal(i-k:i+k).*gauswin );
end
figure(2), clf, hold on
plot(time,signal,'r')
plot(time,filtsigG,'k','linew',3)
xlabel('Time (s)'), ylabel('amp. (a.u.)')
legend({'Original signal';'Gaussian-filtered'})
title('Gaussian smoothing filter')
filtsigMean = zeros(size(signal));
k = 20; 
for i=k+1:n-k-1
    filtsigMean(i) = mean(signal(i-k:i+k));
end
plot(time,filtsigMean,'b','linew',2)
legend({'Original signal';'Gaussian-filtered';'Running mean'})
zoom on

