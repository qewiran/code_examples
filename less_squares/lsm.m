function lsmPoly = lsm(tblX, tblY, order)
% составляем полином вида c_0q_0+c_1q_1+...+c_order q_order
% где (q_0,...,q_order) ортогональный базис
nodeCount = length(tblX);

fourierCoeffs = zeros(1,order+1); % это коэффициенты c_i
polyBasis = polyOrthogonalBasis(order,tblX); % это базис полиномов
lsmPoly = 0;

for i=1:order+1
    if (i==1)
        fourierCoeffs(i) = sum(tblY) / nodeCount;
    else
        nom = 0; den = 0;
        for j=1:nodeCount
            nom = nom + polyval(polyBasis{i},tblX(j)) * tblY(j);
            den = den + polyval(polyBasis{i},tblX(j)) ^ 2;
        end
        fourierCoeffs(i) = nom / den;
    end
    lsmPoly = polyAdd(lsmPoly,fourierCoeffs(i) .* polyBasis{i});
end

end