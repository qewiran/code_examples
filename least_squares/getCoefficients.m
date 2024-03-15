function [a_val,b_val] = getCoefficients (polyCurr, polyPrev,tblX)

% вычисление коэффициентов a_k+1 = <x*q_k, q_k> / <q_k,q_k>, <.,.> — скал. пр-е
% b_k = <x*q_k,q*q_{k-1}>/<q_{k-1},q_{k-1}>
nodeCount = length(tblX);
a_nom =0; a_den =0;
b_nom =0; b_den =0;
for j=1:nodeCount
    a_nom = a_nom + tblX(j)*polyval(polyCurr,tblX(j))^2;
    a_den = a_den + polyval(polyCurr,tblX(j))^2;

    b_nom = b_nom + tblX(j)*polyval(polyCurr,tblX(j))*polyval(polyPrev,tblX(j));
    b_den = b_den + polyval(polyPrev,tblX(j))^2;
end
a_val = a_nom/a_den;
b_val = b_nom/b_den;
end