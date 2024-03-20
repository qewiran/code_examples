function resultPoly = butterworthPoly(order)

resultPoly = [1 -2*cos((2*1 + order - 1) / (2 * order) * pi) 1];

for k=2:(order / 2)
    resultPoly = conv(resultPoly, [1 -2*cos((2*k + order - 1) / (2 * order) * pi) 1]);
end

if mod(order,2)==1
    resultPoly = conv(resultPoly,[1 1]);
end

end