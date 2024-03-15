f = @(x) exp(x) .* (x-2) .^2; % тестовые значения будем брать для этой функции

tblX = -1:0.1:1; % узлы (>5 штук для базиса из 6-ти полиномов)
tblY = f(tblX); % значения функции
nodeCount = length(tblX);
approximationOrder = 5;
x = linspace(-1,1); % для построение графиков


%% МНК по равноотстоящим узлам (узлов >= 5)

figure(1);
clf;
hold on;
scatter(tblX,tblY);
polyLSM = lsm(tblX,tblY,approximationOrder)

plot(x,polyval(p,x));
plot(x,f(x));

legend('exact data','LSM 5th order basis', '$e^x(x-2)^2$ ', 'Interpreter', 'Latex');
hold off;


figure(2);
clf;
hold on;
err = rand(1,nodeCount, "double"); % генерация ошибок 
err_sign = 2*randi([0, 1],1,nodeCount)-1; % генерация случайных знаков (+1,-1)
err = err_sign.*err;
tblY_errs = tblY;
for j=1:nodeCount
    tblY_errs(j) = tblY_errs(j) * ( 1 + 0.05*err(j)); % 5 процентов погрешность
end

scatter(tblX,tblY_errs);

polyLSMerr = lsm(tblX,tblY_errs,approximationOrder)
plot(x,polyval(polyLSMerr,x));
plot(x,f(x));

legend('data with 5 percent error','LSM 5th order basis', '$e^x(x-2)^2$', 'Interpreter', 'Latex');
hold off;
