% function to extract audio files
% place audio to be extracted in same directory as this function
% Example use AudioExtractor('audio.mp3', 'audio1');
% you will get an audio1.c file and audio1.h file with variable name audio1
function AudioExtractor(inputFileName, variableName)

    % read the audio file
    [audioData, SR] = audioread(inputFileName);
    % get the array size
    [row,col] = size(audioData);

    % If stereo (2 channels)
    if col == 2
        % Create C file
        fd=fopen([variableName '.c'],'wt');
        % print variable name
        fprintf(fd,['const double ' variableName ' [%d][%d]={'],row, col);    
        % begin printing the array
        for chunkStart = 1:1:(row-1)
            fprintf(fd,'\n ');
            fprintf(fd,' {%d, %d},',audioData(chunkStart, 1), audioData(chunkStart, 2));
        end
        fprintf(fd,'\n ');
        fprintf(fd,' {%d, %d}',audioData(row, 1), audioData(row, 2));
        fprintf(fd,'\n }; \n');
        fclose(fd);        

        % Create H file
        fd=fopen([variableName '.h'],'wt');
        fprintf(fd,['#ifndef ' upper(variableName) '_H_\n#define ' upper(variableName) '_H_\n']);    
        fprintf(fd,['extern const double ' variableName ' [%d][%d];\n'],row, col);    
        fprintf(fd,'#endif \n');
        fprintf(fd,'\n ');
        fclose(fd);        
        
    end
    
    % if mono (single channel)
    if col == 1
        %c file
        fd=fopen([variableName '.c'],'wt');
        fprintf(fd,['const double ' variableName ' [%d]={'],row);    
        for chunkStart = 1:1:(row-1)
            fprintf(fd,'\n ');
            fprintf(fd,' %d,',audioData(chunkStart));
        end
        fprintf(fd,'\n ');
        fprintf(fd,' %d};\n',audioData(row));
        fprintf(fd,'\n ');
        fclose(fd);
        
        % Create H file
        fd=fopen([variableName '.h'],'wt');
        fprintf(fd,['#ifndef ' upper(variableName) '_H_\n#define ' upper(variableName) '_H_\n']);    
        fprintf(fd,['extern const double ' variableName ' [%d];\n'],row);
        fprintf(fd,'#endif \n '); 
        fprintf(fd,'\n ');
        fclose(fd);                
    end

end

