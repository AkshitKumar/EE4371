x = [0 1 2 1 0]; % Get the X coordinates of all the vertices
y = [0 0 1 2 2]; % Get the Y coordinates of all the vertices
% Complete the polygon
for i = 1:4
line([x(i),x((i+1))],[y(i),y(i+1)])
end
line([x(5),x(1)],[y(5),y(1)])

% Plotting the answer
i = [1 2 2];
j = [2 4 3];
k = [5 5 4];

for p = 1:3
    line([x(i(p)),x(j(p))],[y(i(p)),y(j(p))])
    line([x(j(p)),x(k(p))],[y(j(p)),y(k(p))])
    line([x(k(p)),x(i(p))],[y(k(p)),y(i(p))])
end
