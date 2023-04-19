//
// parse directory tree from "/"(root) for finding files with specified extension
// recursive function
//
// in: directory: SD.open file
//     directoryName: Search start directory name that same with SD.open("/");
//       ex. File file = SD.open("/F1");
//           filecount = directoryParser(file, "/F1/", fileExtension);
//           extension: searchig file name that has extention name (".txt")
//           buffer: detected all file paths. (return)
// out: how many files are detected
//

int directoryParser(File directory, String searchStartDirectoryName, String extension, char * buffer)
{
  int filecount = 0;   // return value
  String filename;        // openNextFile name
  while (true)
  {
    File file = directory.openNextFile();
    if (!file)  { break; }               // no file or directory 
    filename = file.name();
    if (file.isDirectory()) {            // found Directory 
          String prevDirectory;
          prevDirectory = searchStartDirectoryName;
          searchStartDirectoryName += filename;
          searchStartDirectoryName += "/";
          filecount += directoryParser(file, searchStartDirectoryName, extension, buffer);
          searchStartDirectoryName = prevDirectory;
        }
        else {                            // found file
            String ext = filename.substring(filename.lastIndexOf('.'));
            if (filename[0] == '_' or !isAlphaNumeric(filename[0])) { continue; }
            if (ext.equalsIgnoreCase(extension)) { 
                 // SerialUSB.print(searchStartDirectoryName);
                 // SerialUSB.println(filename);
                  strcat(buffer, &searchStartDirectoryName[0]);
                  strcat(buffer, &filename[0]);
                  strcat(buffer, "\n");
                  filecount++;
              }
        }
      file.close();
  }
  return filecount;
}