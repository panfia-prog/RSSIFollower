% For clearing command window and opened windows
clc;
close all;

% Setting up the TCP client, which is how Arduino and Matlab communicate
esp32Ip = "192.168.4.1"; % *Change this if the IP address printed in the Serial monitor is different
port = 80;
t = tcpclient(esp32Ip,port);

% Setting up arrays and flag 
rssiArray = [];
rssiCollectionFlag = 1;
timeVector = [];

% Starts a runtime timer 
tic;

% Runs until RSSI values for the 3 directions are  collected
while rssiCollectionFlag ~= 4
    % Checks if the array has 60 values
    if(size(rssiArray)<=59)
        % Checks if data is being sent
        if t.NumBytesAvailable>0
            % Reads the RSSI data sent and stores it in rssiVal
            textData=readline(t)
            rssiVal = str2double(textData);
            % Gets another RSSI value if the value returned is NaN
                while isnan(rssiVal)
                    textData=readline(t)
                    rssiVal = str2double(textData);
                end
            % Stores RSSI value and time when RSSI value was taken in their
            % respective arrays
            rssiArray(end+1) = rssiVal;
            timeVector(end+1) = toc;

            % Displays current RSSI value and size of the array
            disp(['Current RSSI: ', num2str(rssiVal)]);
            disp(size(rssiArray))

            % Brief pause
            pause(0.5);
        end
    % Executes when array has 60 values
    else
        % Sends the character 'd' to the Arduino
        write(t,'d','char');
        % Stores rssiArray and timeVector in a timeseries
        ts = timeseries(rssiArray,timeVector);
        % Denoises rssiArray using wavelet denoiser
        rssiArraydenoised = wdenoise(rssiArray,3, ...
            Wavelet='sym5', ...
            DenoisingMethod='UniversalThreshold', ...
            ThresholdRule='Soft', ...
            NoiseEstimate='LevelDependent');

        % Stores the value in respective direction's array
        % Displays what direction's array is obtained for debugging
        switch rssiCollectionFlag
            case 1
                rnoisy = rssiArray;
                rsmoothed = rssiArraydenoised;
                disp('RIGHT obtained');
            case 2
                fnoisy = rssiArray;
                fsmoothed = rssiArraydenoised;
                disp('FORWARD obtained');
            case 3
                lnoisy = rssiArray;
                lsmoothed = rssiArraydenoised;
                disp('LEFT obtained');
        end

        % Resets rssiArray and timeVector for next direction
        % Increments rssiCollectionFlag
        rssiArray = []; 
        timeVector = []; 
        rssiCollectionFlag = rssiCollectionFlag + 1;
    end
end

% After collecting all the data, Signal Analyzer app will be opened
% Displays a graph of the noisy and smoothed data for all directions
signalAnalyzer(rnoisy);
signalAnalyzer(rsmoothed);
signalAnalyzer(fnoisy);
signalAnalyzer(fsmoothed);
signalAnalyzer(lnoisy);
signalAnalyzer(lsmoothed);

% Collects the peak to peak value of the smoothed signals
p2pval = [peak2peak(rsmoothed),peak2peak(fsmoothed),peak2peak(lsmoothed)];
% Finds lowest peak to peak value
[~, minI] = min(p2pval);

% Displays the final direction judgement
switch minI
    case 1
        disp('Moving RIGHT...');
    case 2
        disp('Moving FORWARD...');
    case 3
        disp('Moving LEFT...');
    otherwise
        disp('Undecided. Moving FORWARD by default...');
end

clear t;