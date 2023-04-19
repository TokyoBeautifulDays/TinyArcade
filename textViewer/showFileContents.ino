//
// 
#define XOFFSET 0
#define YOFFSET 0

#define topDirectory "/"
#define TERM '\0'


void showFileContents(String fileExtension) {
  
#define TEXTSIZE 1000
  char buffer[TEXTSIZE];

  int filecount = 0, displayedLines = 0;
  
    if (!SD.begin()) { display.println("SD begin Error!\n"); return; }
    File file = SD.open(topDirectory); file.rewindDirectory();

    if (file) {
        buffer[0] = TERM;  // initialize
        filecount = directoryParser(file, topDirectory, fileExtension, buffer);

        char filename [100];
        int bufferPointer = 0, files = 0;
        while(1) {
          // display file name circulately. reset file count and file buffer pointer
          if (files >= filecount) break;

          // extract next file name from buffer
          filename[0] = TERM; int length = 0;
          while (buffer[bufferPointer] != '\n') { filename[length++] = buffer[bufferPointer++]; }
          filename[length] = TERM;
          bufferPointer++;   // point first char of next file name in buffer
         
         // show file name
          displayBufferCharCount(&filename[0], length, XOFFSET, YOFFSET);

          // user operation (OK: show Text, skip: next file)
          display.setCursor(0,SCREEN_HEIGHT - display.getFontHeight());
          display.print("OK..Btn1  Skip..Btn2");
          while(1) {
               // show content of file or skip file
               if (checkButton(TAButton1)) { showText(filename, XOFFSET, YOFFSET); break; }
               if (checkButton(TAButton2)) { delay(500); break; }
          }
          // clear up screen and point next file
          display.clearScreen(); files ++;
        }     
    } else { display.println("Root Directory open Error"); return; }
}
// show file content
void showText(String filename, int xOffset, int yOffset) {
    int size = 0;
    char text[TEXTSIZE];
    int displayedCharCount = 0; 

    text[0] = TERM;
    size = fileRead2Buffer(text, filename);
    if (size <= 0) return;

    while(size != displayedCharCount) {
        display.clearScreen(); 
        displayedCharCount +=  displayBufferCharCount(&text[displayedCharCount], (size - displayedCharCount), xOffset, yOffset);
        while(button2ReadOut() < THRESHOLD) {}
    }
}