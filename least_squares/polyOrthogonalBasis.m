function basis = polyOrthogonalBasis(order, tblX)
% процедура получения ортогональных многочленов до порядка order
nodeCount = length(tblX);
basis = cell(1,order+1);
basis{1} = 1;
basis{2} = [1 -sum(tblX)/nodeCount];
for i=3:order+1
    [a,b] = getCoefficients(basis{i-1},basis{i-2},tblX);
    for j=1:nodeCount
        basis{i} = polyAdd(polyAdd(conv([1 0],basis{i-1}),-a.*basis{i-1}),-b.*basis{i-2});
    end
end

end