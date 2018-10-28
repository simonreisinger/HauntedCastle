%{
https://www.cs.cmu.edu/afs/andrew/scs/cs/15-463/f07/proj1/www/wwedler/
Authors: Reisinger, 1426220@student.tuwien.ac.at
%}
function imgOutput = main()
pkg load signal % Octave only
    iVersion = char('', 'b', 'm', 'n', 's')
    for i =  1: size(iVersion, 1)
        i
        x = iVersion(i)
        buildImage(x)
    end
end

function correctRGBImg = buildImage(x)
    fileLocation = 'data/';
    fileResutlLocation = 'result/';
    imagesName = {'stone01', 'wood_09', 'wood_10'};
    % result = zeros(1)
    for idx = imagesName
        id = idx{1,1}

        filename = strcat(id, x, '.jpg')
        loc = strcat(fileLocation, filename)
        currentImages = imread(loc);
        dimension = size(size(currentImages), 2)

        if strcmp(idx, 'wood_10') % downscales Image with this name
            size(currentImages)
            xyz = dimension
            if dimension == 2 % grey image
                uImages =        downsample(downsample(currentImages(:,:),2)',2)';
            elseif dimension == 3
                size(currentImages(:,:,1))
                size(downsample(currentImages(:,:,1),2))
                uImages =        downsample(downsample(currentImages(:,:,1),2)',2)';
                uImages(:,:,2) = downsample(downsample(currentImages(:,:,2),2)',2)';
                uImages(:,:,3) = downsample(downsample(currentImages(:,:,3),2)',2)';
            else
                e = "ERROR"
            end
            currentImages = uImages;
            size(currentImages)
        end

        repetedImage = repmat(currentImages, 3, 6);

        if (exist("result", "var") == 0)
            result = repetedImage;
            size(result)
        elseif dimension == 2 % grey image
            currentDimesions = size(repetedImage)
            A = result;
            B = repetedImage;
            A(size(A,1)+1:size(A,1)+size(B,1),1:size(B,2))=B;
        elseif dimension == 3
            A = result;
            B = repetedImage;
            result(size(A,1)+1:size(A,1)+size(B,1),1:size(B,2),:)=B;
        else
            e = "ERROR"
        end

        label = strcat(fileResutlLocation, filename);
        imwrite(repetedImage, label);

    end
    %figure;
    %imshow(result)
    %imwrite(result, strcat(fileResutlLocation, "Combined", x, ".jpg"));
end