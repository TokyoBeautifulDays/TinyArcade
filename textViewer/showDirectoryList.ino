// Directory and file list

#define topDirectory "/"
#define TERM '\0'

void showDirectoryList(String fileExtension) {

#define TEXTSIZE 1000
    char buffer[TEXTSIZE];
    int filecount = 0;
    int displayedFileCount = 0;
    uint16_t index = 0;
    uint8_t count = 0;  // file count
  
    if (!SD.begin()) {
       DEBUGprintString("SD begin Error!\n");
       return;
       }
    File file = SD.open(topDirectory);
    file.rewindDirectory();
    if (file) {
        buffer[0] = TERM;  // initialize
        filecount = directoryParser(file, topDirectory, fileExtension, buffer);
        int len = strlen(buffer);
        while(filecount > displayedFileCount) {

            display.clearScreen();
            displayedFileCount += displayBufferFileCount(&buffer[index], len, 0, 0);

            while((button1ReadOut() < THRESHOLD) and (button2ReadOut() < THRESHOLD)) {}  // wait 
            while (displayedFileCount > count) {
                if (buffer[index] == '\n') count++;
                index++; len--;
            }
        }
        display.clearScreen();
        
    } else { 
        DEBUGprintString("Root Directory open Error");
        return;
    }
}