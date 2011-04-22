

using namespace std;

class schedule 
{
    BYTE id;
    list<job*> job_list;
    
    WORD cpListenerPortNum;

	errType convertToCronTask(job* newJob, cronTask *task);
    public:
    
     schedule();
    ~schedule();
    
    errType addJob(job* );
    job*  getJobById(DWORD id);
    job*  getJobByIndex(DWORD index);
    WORD getJobsQuantity();

    errType cursorPos(BYTE **ids);
    errType removeAllJobsBefore(DWORD );

    errType run();
    errType stop();

    errType decode(BYTE*);
    errType encode(BYTE*);

    void set_cpListenerPortNum(WORD);
    WORD get_cpListenerPortNum();

    errType execute(DWORD);
    errType clear();
    void dbgPrint();


};

