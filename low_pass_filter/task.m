gainPass = -3;
gainStop = -25;

freqPass = 20;
freqStop = 50;

filter = butterworthFilter(freqPass,freqStop,gainPass,gainStop);