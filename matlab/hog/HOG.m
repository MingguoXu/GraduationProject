%%
clear all;
close all;
clc;

%%
img = imread('lena.jpg');

%% 1���ҶȻ�
img = rgb2gray(img);
img = double(img);

step = 8;   % step*step��������Ϊһ��cell
[m1, n1] = size(img);

%�ı�ͼ��ߴ�Ϊstep�����������
img = imresize(img, [floor(m1/step)*step, floor(n1/step)*step], 'nearest');
[m, n] = size(img);

%% 2��٤��У��
% GammaУ��ǰͼƬ
% figure();
% imshow(img, []);

img = sqrt(img);

% GammaУ����ͼƬ
% figure();
% imshow(img, []);

%% 3�����ݶȺͷ���
fy = [-1, 0, 1];    %������ֱģ��
fx = fy';           %����ˮƽģ��
Iy = imfilter(img, fy, 'replicate');    %��ֱ�ݶ�
Ix = imfilter(img, fx, 'replicate');    %ˮƽ�ݶ�
Ied = sqrt(Ix.^2 + Iy.^2);              %�ݶ�ֵ
Iphase = Iy ./ Ix;      %��Եб�ʣ���ЩΪinf,-inf,nan������nan��Ҫ�ٴ���һ��
the = atan(Iphase) * 180 / pi;

% ����õ����ݶ�ͼ
% figure();
% imshow(Ied, []);

for i = 1:m
    for j = 1:n
        if(Ix(i,j)>=0&Iy(i,j)>=0)
            the(i,j) = the(i,j);
        elseif(Ix(i,j)<=0&Iy(i,j)>=0)
            the(i,j) = the(i,j) + 180;
        elseif(Ix(i,j)<=0&Iy(i,j)<=0)
            the(i,j) = the(i,j) + 180;
        elseif(Ix(i,j)>=0&Iy(i,j)<=0)
            the(i,j) = the(i,j) + 360;
        end

        if isnan(the(i,j)) == 1
            the(i,j) = 0;
        end
    end
end
the = the + 0.000001;%��ֹ�Ƕ�Ϊ0

%% 4������cell����cell��ֱ��ͼ( 1 cell = 8*8 pixel )
step=8;                %step*step��������Ϊһ��cell
orient=9;               %����ֱ��ͼ�ķ������
angle_step = 360 / orient;%ÿ����������ĽǶ���
Cell = cell(1,1);        %���еĽǶ�ֱ��ͼ,cell�ǿ��Զ�̬���ӵģ�����������һ��
i = 1;
j = 1;
ii = 1;
jj = 1;
for i = 1:step:m
    ii = 1;
    for j = 1:step:n
        Hist1(1:orient) = 0;
        for p = 1:step
            for q = 1:step
                %�ݶȷ���ֱ��ͼ
                Hist1(ceil(the(i+p-1, j+q-1)/angle_step)) = Hist1(ceil(the(i+p-1, j+q-1)/angle_step)) + Ied(i+p-1, j+q-1);
            end
        end
        Cell{ii, jj} = Hist1;
        ii = ii + 1;
    end
    jj = jj + 1;
end

%% 5������block����block������ֵ��ʹ���ص���ʽ( 1 block = 2*2 cell )
[m, n] = size(Cell);
feature = cell(1, (m-1)*(n-1));
for i=1:m-1
    for j=1:n-1
        block = [];
        block = [Cell{i,j}(:)' Cell{i,j+1}(:)' Cell{i+1,j}(:)' Cell{i+1,j+1}(:)'];
        block = block ./ sum(block);%��һ��
        feature{(i-1)*(n-1)+j} = block;
    end
end

%% 6��ͼ���HOG����ֵ
[m, n] = size(feature);
l = 2 * 2 * orient;
featureVec = zeros(1, n * l);
for i = 1:n
    featureVec((i-1)*l+1:i*l) = feature{i}(:);
end