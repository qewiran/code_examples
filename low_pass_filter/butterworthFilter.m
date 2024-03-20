function TF_final = butterworthFilter(freqPass, freqStop, gainPass, gainStop)

filterOrder = log10((10^(-gainStop / 10) - 1) / (10^(-gainPass / 10) - 1))...
    / (2 * log10(freqStop / freqPass));

filterOrder = ceil(filterOrder);

freqCutoff1 = freqPass / (10^(- gainPass / 10) - 1)^(1 / (2*filterOrder));
freqCutoff2 = freqStop / (10^(- gainStop / 10) - 1)^(1 / (2*filterOrder));

freqCutoff = min(freqCutoff1, freqCutoff2);

denominatorTF = butterworthPoly(filterOrder);
% TF_normalized  = tf(1,denominatorTF);

for k=filterOrder:-1:1
    denominatorTF(filterOrder-k+1) = denominatorTF(filterOrder-k+1) * ...
        (1/freqCutoff)^k;
end
TF_final = tf(1,denominatorTF);

