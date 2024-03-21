t = (0:0.1:60)';
x1 = sawtooth(t);

y1 = awgn(x1,10);

figure('Name','Test function','NumberTitle','off');
plot(t,[x1 y1]);
legend('зубчатая тестовая ф-я','зубчатая ф-я с белым шумом');

gainPass1 = -10;
gainStop1 = -40;

freqPass1 = 6.5;
freqStop1 = 200;

figure('Name','Bode plot','NumberTitle','off');
clf;
hold on;
filter1 = butterworthFilter(freqPass1,freqStop1,gainPass1,gainStop1);

opts = bodeoptions;
opts.Title.String = 'ЛАЧХ и ЛФЧХ фильтра';
opts.Title.FontSize = 20;
bode(filter1,opts);

grid on;
hold off;

figure('Name','Filtered function','NumberTitle','off');
clf;
[y1_out, time1] = lsim(filter1,y1,t);
hold on;
plot(time1,y1_out,"Color",'r');
plot(time1,x,"Color",'b');
hold off;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
x2 = square(t);
y2 = awgn(x2,10);

figure('Name','Test function','NumberTitle','off');
plot(t,[x2 y2]);
legend('квадратный импульс','квадратный импульс с белым шумом');

gainPass2 = -2;
gainStop2 = -50;

freqPass2 = 2.6;
freqStop2 = 300;

figure('Name','Bode plot','NumberTitle','off');
clf;
hold on;
filter2 = butterworthFilter(freqPass2,freqStop2,gainPass2,gainStop2);

opts = bodeoptions;
opts.Title.String = 'ЛАЧХ и ЛФЧХ фильтра';
opts.Title.FontSize = 20;
bode(filter1,opts);


figure('Name','Filtered function','NumberTitle','off');

clf;
[y2_out, time2] = lsim(filter2,y2,t);
hold on;
plot(time2,y2_out,"Color",'r');
plot(time2,x2,"Color",'b');
hold off;

