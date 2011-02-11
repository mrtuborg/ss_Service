
using namespace std;

class schedule 
{
    BYTE id;
    cronTab* cronJob;
    list<job*> job_list;
    
    int fd;  // mapping file descriptor;
    
    public:
    
     schedule();
    ~schedule();
    
    errType addJob(job* );
    errType removeAllJobsBefore(DWORD );
    errType execJob();
    errType checkAlarm();
    errType update();
    errType mappingOpen(BYTE*);
    errType mappingClose();
    errType decode(BYTE*);
    errType encode(BYTE*);
	errType writeCronTab(job*);
};

