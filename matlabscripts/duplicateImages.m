%{
https://www.cs.cmu.edu/afs/andrew/scs/cs/15-463/f07/proj1/www/wwedler/
Authors: Reisinger, 1426220@student.tuwien.ac.at
%}
function imgOutput = main(debugFlag)
    if ~exist('debugFlag')
        debugFlag = false; %TODO change to false before handing in
    end
    if debugFlag == false
        imagesName = {'stone01', 'wood_09', 'wood_10'};
    else
        imagesName = {'stone01'};
    end

    for i = imagesName
        buildImage(i{1,1})
    end
end

function correctRGBImg = buildImage(id)
    fileLocation = 'data/';
    fileResutlLocation = 'result/';

    iVersion = char('', 'b', 'm', 'n', 's');
    for i =  1: size(iVersion, 1)
        x = iVersion(i)
        filename = strcat(id, x, '.jpg')
        loc = strcat(fileLocation, filename)
        currentImages = imread(loc);
        size(currentImages);
        repetedImage = repmat(currentImages, 3, 6);
        size(repetedImage)
        label = strcat(fileResutlLocation, filename);
        %figure;
        %imshow(repetedImage); % octave flips image by 180 degrees
        %title(label);
        yyyy = 'vor'
        imwrite(repetedImage, label);
        yyyy = 'nach'
    end
end