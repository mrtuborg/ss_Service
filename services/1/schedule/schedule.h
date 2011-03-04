
using namespace std;

class schedule 
{
    BYTE id;
    cronTab* cronJob;
    list<job*> job_list;
    

	errType writeCronTab(job*);
    public:
    
     schedule();
    ~schedule();
    
    errType addJob(job* );
    errType removeAllJobsBefore(DWORD );

    errType run();
    errType stop();

    errType decode(BYTE*);
    errType encode(BYTE*);


};

