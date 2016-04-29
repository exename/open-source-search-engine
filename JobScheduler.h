#ifndef JOBSCEHDULER_H_
#define JOBSCEHDULER_H_

#include <inttypes.h>

class BigFile;
class FileState;

//exit codes/reasons when the finish callback is called
enum job_exit_t {
	job_exit_normal,	        //job ran to completion (possibly with errors)
	job_exit_cancelled,             //job was cancelled (typically due to file being removed)
	job_exit_deadline,	        //job was cancelled due to deadline
	job_exit_program_exit	        //job was cancelled because program is shutting down
};


typedef void (*start_routine_t)(void *state);
typedef void (*finish_routine_t)(void *state, job_exit_t exit_type);


enum thread_type_t {
	thread_type_query_read,
	thread_type_query_constrain,
	thread_type_query_merge,
	thread_type_query_intersect,
	thread_type_query_summary,
	thread_type_spider_read,
	thread_type_spider_write,
	thread_type_spider_filter,      //pdf2html/doc2html/...
	thread_type_spider_query,       //?
	thread_type_replicate_write,
	thread_type_replicate_read,
	thread_type_file_merge,
	thread_type_file_meta_data,     //unlink/rename
	thread_type_statistics,         //mostly i/o
	thread_type_unspecified_io,     //until we can be more specific
	thread_type_unlink,             //unlnk()
	thread_type_twin_sync,
	thread_type_hdtemp,
	thread_type_generate_thumbnail,
};


class JobScheduler_impl;

typedef void (*job_done_notify_t)();

class JobScheduler {
	JobScheduler(const JobScheduler&);
	JobScheduler& operator=(const JobScheduler&);
public:
	JobScheduler();
	~JobScheduler();
	
	bool initialize(unsigned num_cpu_threads, unsigned num_io_threads, unsigned num_external_threads, job_done_notify_t job_done_notify=0);
	void finalize();
	
	bool submit(start_routine_t   start_routine,
	            finish_routine_t  finish_callback,
		    void             *state,
		    thread_type_t     thread_type,
		    int               priority,
		    uint64_t          start_deadline=0);
	bool submit_io(start_routine_t   start_routine,
	               finish_routine_t  finish_callback,
		       FileState        *fstate,
		       thread_type_t     thread_type,
		       int               priority,
		       bool              is_write_job,
		       uint64_t          start_deadline=0);
	
	bool are_io_write_jobs_running() const;
	void cancel_file_read_jobs(const BigFile *bf);
	//void nice page for html and administation()
	bool is_reading_file(const BigFile *bf);
	
	void allow_new_jobs();
	void disallow_new_jobs();
	bool are_new_jobs_allowed() const;
	
	unsigned num_queued_jobs() const;
	
	void cleanup_finished_jobs();
	
private:
	JobScheduler_impl *impl;
};

extern JobScheduler g_jobScheduler;


#endif