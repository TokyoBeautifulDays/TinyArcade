//
// open file and read data into buffer
//
// in: buffer: read area (text data)
//     filename: file name on SD card
// out:  buffer size that reads
//

int fileRead2Buffer(char * buffer, String filename ) {
    if (!SD.begin()) {
       DEBUGprintString("SD begin Error!\n");
       return 0;
       }
    File file = SD.open(filename, FILE_READ);
    if (file) {
      int index = 0;
      while ( file.available() ) {
         buffer[index++] = char(file.read());
     }
     file.close();
     return index;
    } else { 
      DEBUGprintString("File open Error");
      return 0;
    }
}