clc; clear; close all;

% ----- KEEP NUM_SAMPLES UPDATED -----
NUMBER_SAMPLES = 1000;

fid = fopen('CollectedValues.csv','r');

% Initializing samples
samples = zeros([NUMBER_SAMPLES,2]);
i = 1;

% Reading the file and putting the values into the sample matrix
while ~feof(fid)
    line = fgetl(fid);
    token = textscan(line,'%d %f','delimiter', ',');
    samples(i,1) = token{1};
    samples(i,2) = token{2};
    i = i+1;
end

fclose('all');

samples = samples';