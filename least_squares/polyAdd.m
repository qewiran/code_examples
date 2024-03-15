function resultPoly = polyAdd(poly1,poly2)
poly1_order = length(poly1);
poly2_order = length(poly2);

max_order = max(poly1_order,poly2_order);

new_poly1 = [zeros(1, max_order - poly1_order),poly1];

new_poly2 = [zeros(1, max_order - poly2_order),poly2];

resultPoly = new_poly1 + new_poly2;

end