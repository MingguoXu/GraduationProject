%1(start /min cmd.exe /c %0 :&exit)
call javac -encoding UTF-8 Server.java
call java Server