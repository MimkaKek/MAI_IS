import time

class Logger():

    def __init__(self):
        self.logFilename = "crawler.log"
        pass

    def getTimeString(self):
        t = time.localtime()
        strMon  =  "0" + str(t.tm_mon) if (t.tm_mon < 10) else str(t.tm_mon)
        strDay  =  "0" + str(t.tm_mday) if (t.tm_mday < 10) else str(t.tm_mday)
        strHour =  "0" + str(t.tm_hour) if (t.tm_hour < 10) else str(t.tm_hour)
        strMin  =  "0" + str(t.tm_min) if (t.tm_min < 10) else str(t.tm_min)
        strSec  =  "0" + str(t.tm_sec) if (t.tm_sec < 10) else str(t.tm_sec)
        return str(t.tm_year) + "/" + strMon + "/" + strDay + " | " + strHour + ":" + strMin + ":" + strSec
    
    def debug(self, str):
        with open(self.logFilename, "a") as file:
            file.write("[ " + self.getTimeString() + " ] " + "[ DEBUG ] >" + str + "\n")

    def info(self, str):
        with open(self.logFilename, "a") as file:
            file.write("[ " + self.getTimeString() + " ] " + "[ INFO  ] > " + str + "\n")

    def error(self, str):
        with open(self.logFilename, "a") as file:
            file.write("[ " + self.getTimeString() + " ] " + "[ ERROR ] > " + str + "\n")

    def warn(self, str):
        with open(self.logFilename, "a") as file:
            file.write("[ " + self.getTimeString() + " ] " + "[ WARN  ] > " + str + "\n")