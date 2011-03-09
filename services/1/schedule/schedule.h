
using namespace std;

class schedule 
{
    BYTE id;
   /// cronTab* cronJob; -- dynamicaly created at run command
    list<job*> job_list;
    

	errType convertToCronTask(job* newJob, cronTask *task);
    public:
    
     schedule();
    ~schedule();
    
    errType addJob(job* );
    errType removeAllJobsBefore(DWORD );

    errType run();
    errType stop();

    errType decode(BYTE*);
    errType encode(BYTE*);


    void dbgPrint();

};

