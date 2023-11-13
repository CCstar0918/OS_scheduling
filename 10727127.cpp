#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
fstream infile, outfile ;
struct process{
  int ID ;
  int cpu_burst ;
  int cpu ;
  int arrival_time ;
  int priority ;
  int start_time ;
  int end_time ;
  double ratio ;
};
bool LoadFile(string &fileName) {
	fileName = fileName + ".txt" ; 
	infile.open(fileName.c_str(), ios::in ) ;  
	if ( infile == NULL ) {
		cout << endl << "### " << fileName << " does not exist !! " << " ###"<< endl ;
		infile.close() ;
		return false ;
	} // if

	else {
		return true ;
	} // else
} // LoadFile()

void StoreFile( int &mode, int &timeslice, vector<process> &all_process ) {
  string t ;
	infile >> mode ;
	infile >> timeslice ;
	int count = 0 ;
	process temp ;
	while ( infile >> t ) {
	  if( count < 6 ){
	    count++ ;
    } // if
    
    else {
      temp.ID = atoi( t.c_str() ) ;
      infile >> temp.cpu_burst ;
      infile >> temp.arrival_time ;
      infile >> temp.priority ;
      temp.start_time = -1 ;
      temp.end_time = -1 ;
      temp.cpu = temp.cpu_burst ;
      all_process.push_back(temp) ;
    } // else
	} // while
	
	infile.close() ;
} // StoreFile()


void Sort( vector<process> &all_process) {
  bool sorted = false ;
	for ( int i = 0 ; i < all_process.size(); i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < all_process.size() - i - 1 ; j++ ) {
			if ( all_process[j].arrival_time > all_process[j + 1].arrival_time ) {
				swap( all_process[j], all_process[j+1] ) ;
				sorted = false ;
			} // if
			
			else if ( all_process[j].arrival_time == all_process[j + 1].arrival_time ) {
			  if ( all_process[j].ID > all_process[j + 1].ID ) {
			    swap( all_process[j], all_process[j+1] ) ;
				  sorted = false ;
        } // if
      } // else if
		} // for
		
		if(sorted)
			break ;
	} // for
} // Sort()

void IDSort( vector<process> &all_process) {
  bool sorted = false ;
	for ( int i = 0 ; i < all_process.size(); i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < all_process.size() - i - 1 ; j++ ) {
			if ( all_process[j].ID > all_process[j + 1].ID ) {
				swap( all_process[j], all_process[j+1] ) ;
				sorted = false ;
			} // if
		} // for
		
		if(sorted)
			break ;
	} // for
} // IDSort()

void sort_burst( vector<process> &all_process ) {
  bool sorted = false ;
	for ( int i = 0 ; i < all_process.size(); i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < all_process.size() - i - 1 ; j++ ) {
			if ( all_process[j].cpu_burst > all_process[j + 1].cpu_burst ) {
				swap( all_process[j], all_process[j+1] ) ;
				sorted = false ;
			} // if
			
			else if ( all_process[j].cpu_burst == all_process[j + 1].cpu_burst ) { // burst == burst
			  if ( all_process[j].arrival_time > all_process[j + 1].arrival_time ) { // compare arrival_time
			    swap( all_process[j], all_process[j+1] ) ;
				  sorted = false ;
        } // if
        
        else if ( all_process[j].arrival_time == all_process[j + 1].arrival_time ) {
          if ( all_process[j].ID > all_process[j + 1].ID ) {
			      swap( all_process[j], all_process[j+1] ) ;
				    sorted = false ;
          } // if 
        } // else if
      } // else if
		} // for
		
		if(sorted)
			break ;
	} // for
} // sort_burst()

void sort_ratio( vector<process> &all_process, int count ) {
  
  for ( int i = 0 ; i < all_process.size() ; i++ ) 
    all_process[i].ratio = ( count - all_process[i].arrival_time + all_process[i].cpu_burst ) * 1.0 / all_process[i].cpu_burst   ;
  bool sorted = false ;
	for ( int i = 0 ; i < all_process.size(); i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < all_process.size() - i - 1 ; j++ ) {
			if ( all_process[j].ratio < all_process[j + 1].ratio ) {
				swap( all_process[j], all_process[j+1] ) ;
				sorted = false ;
			} // if
			
			else if ( all_process[j].ratio == all_process[j + 1].ratio ) { // burst == burst
			  if ( all_process[j].arrival_time > all_process[j + 1].arrival_time ) { // compare arrival_time
			    swap( all_process[j], all_process[j+1] ) ;
				  sorted = false ;
        } // if
        
        else if ( all_process[j].arrival_time == all_process[j + 1].arrival_time ) {
          if ( all_process[j].ID > all_process[j + 1].ID ) {
			      swap( all_process[j], all_process[j+1] ) ;
				    sorted = false ;
          } // if 
        } // else if
      } // else if
		} // for
		
		if(sorted)
			break ;
	} // for
} // sort_ratio()

void sort_priority( vector< vector<process> > &all_process) {
  bool sorted = false ;
	for ( int i = 0 ; i < all_process.size(); i ++ ) {
		sorted = true ;
		for ( int j = 0 ; j < all_process.size() - i - 1 ; j++ ) {
		  if ( all_process[j].front().priority > all_process[j + 1].front().priority ) {
				swap( all_process[j], all_process[j+1] ) ;
				sorted = false ;
			} // if
		} // for
		
		if(sorted)
			break ;
	} // for
} // sort_priority()

char GanttChange( int ID ) {
  if ( ID >= 10 )
    return (ID + 55) ;
  else
    return (ID + 48);
} // GanttChange()


vector<process> FCFS( vector<process> &all_process, string &gantt ) {
	int count = 0;
	int i = 0 ;
	int index = 0 ; // 記錄在all_process 的index 
	vector<process> ans ;
	queue<process> q ;
	process cpu ;
	bool idle = true ;
	while( i < all_process.size() ) { // 當process 做完時 i 才+1 
    if ( index < all_process.size() ) {
      while ( all_process[index].arrival_time == count ) { // 時間到了 將process 放入 queue 
        q.push(all_process[index]) ;
        index++ ;
      } // while
    } // if 
    
    if ( idle ) {
      if ( count != 0 )
	      gantt = gantt + "-" ;
    } // if
	  else { // cpu execution
	    gantt = gantt + GanttChange(cpu.ID) ;
	    cpu.cpu_burst = cpu.cpu_burst - 1 ;
	    if (cpu.cpu_burst == 0) { // execution end
	      cpu.end_time = count ;
	      ans.push_back(cpu) ;
	      idle = true ;
	      i++ ;
	    } // if
    } // else
    
    if ( !q.empty() && idle ) {
      cpu = q.front() ;
      q.pop() ;
      cpu.start_time = count ;
      idle = false ;
    } // if
    count++ ;
  } // while 
  
  return ans ;
}// FCFS()

vector<process> RR( vector<process> &all_process, string &gantt, int timeslice ) {
  int count = 0 ;
	int i = 0 ;
	int index = 0 ; // 記錄在all_process 的index 
	int slice ;
	vector<process> ans ;
	queue<process> q ;
	process cpu ;
	bool idle = true ;
	while( i < all_process.size() ) { // 當process 做完時 i 才+1 
    if ( index < all_process.size() ) {
      while ( all_process[index].arrival_time == count ) { // 時間到了 將process 放入 queue 
        q.push(all_process[index]) ;
        index++ ;
      } // while
    } // if 
    
    if ( idle ) {
      if ( count != 0 )
	      gantt = gantt + "-" ;
    } // if
    
	  else { // cpu execution
	    gantt = gantt + GanttChange(cpu.ID) ;
	    cpu.cpu_burst = cpu.cpu_burst - 1 ;
	    slice-- ;
	    if (cpu.cpu_burst == 0) { // execution end
	      cpu.end_time = count ;
	      ans.push_back(cpu) ;
	      idle = true ;
	      i++ ;
	    } // if
	    
	    else if ( slice == 0 ) {
	      q.push(cpu) ;
	      idle = true ;
      } // else if
    } // else
    
    if ( !q.empty() && idle ) {
      cpu = q.front() ;
      slice = timeslice ;
      q.pop() ;
      if ( cpu.start_time != -1)
        cpu.start_time = count ;
      idle = false ;
    } // if
    count++ ;
  } // while 

  return ans ;
} // RR()

vector<process> SRTF( vector<process> &all_process, string &gantt ) {
	int count = 0;
	int i = 0 ;
	int index = 0 ; // 記錄在all_process 的index 
	vector<process> ans ;
	vector<process> q ;
	process cpu ;
	bool idle = true ;
	while( i < all_process.size() ) { // 當process 做完時 i 才+1 
	  
    if ( index < all_process.size() ) {
      while ( all_process[index].arrival_time == count ) { // 時間到了 將process 放入 queue 
        q.push_back(all_process[index]) ;
        index++ ;
      } // while
    } // if 
    
    if ( idle ) {
      if ( count != 0 )
	      gantt = gantt + "-" ;
    } // if
	  else { // cpu execution
	    gantt = gantt + GanttChange(cpu.ID) ;
	    cpu.cpu_burst = cpu.cpu_burst - 1 ;
	    if (cpu.cpu_burst == 0) { // execution end
	      cpu.end_time = count ;
	      ans.push_back(cpu) ;
	      idle = true ;
	      i++ ;
	    } // if
	    
	    else {
	      q.push_back(cpu) ; // 先把cpu 的process 放回queue
	      idle = true ;
      } // else
    } // else
    
    if ( !q.empty() && idle ) {
      sort_burst(q) ;
      cpu = q.front() ;
      q.erase(q.begin()) ;
      if (cpu.start_time != -1)
        cpu.start_time = count ;
      idle = false ;
    } // if
    count++ ;
  } // while 
  
  return ans ;
}// SRTF()

vector<process> PPRR( vector<process> &all_process, string &gantt, int timeslice ) {
  int count = 0 ;
	int i = 0 ;
	int index = 0 ; // 記錄在all_process 的index 
	int slice = 0;
	int cpu = -1 ;
	bool output = false ;
	vector<process> ans ;
	vector< vector<process> > q ;
	//process cpu ;
	int temp_ID ;
	bool idle = true ;
	bool reput = false ;
	while( i < all_process.size() ) { // 當process 做完時 i 才+1 
	  process temp_process ;
    if ( q.size() == 0 ) {
      if (count!=0)
	      gantt = gantt + "-" ;
    }
    
	  else { // cpu execution
	    reput = false ;
	    output = false ;
	    gantt = gantt + GanttChange(q[0][0].ID) ;
	    q[0][0].cpu_burst = q[0][0].cpu_burst - 1 ;
	    slice-- ;
	    if (q[0][0].cpu_burst == 0) { // execution end
	      output = true ;
	      q[0][0].end_time = count ;
	      ans.push_back(q[0][0]) ;
	      q[0].erase(q[0].begin()) ;
	      if(q[0].size() == 0) 
          q.erase(q.begin()) ;
	      i++ ;
	    } // if
	    
	    else if ( slice == 0 ) {
	      temp_process = q[0][0] ;
	      q[0].erase(q[0].begin()) ;
	      reput = true ;
      } // else if
    } // else
    
    if ( index < all_process.size() ) {
      while ( all_process[index].arrival_time == count ) { // 時間到了 將process 放入 queue 
        bool insert = false ;
        for ( int j = 0 ; j < q.size() ; j++) {
          if ( all_process[index].priority == q[j].front().priority) {
            q[j].push_back(all_process[index]) ;
            insert = true ;
          } // if
        } // for
        
        if (!insert) { // 沒有找到相同的priority 
          vector<process> temp_q ;
          temp_q.push_back(all_process[index]) ;
          q.push_back(temp_q) ;
        } // if
        
        index++ ;
      } // while
    } // if 
    
    if ( reput )
      q[0].push_back(temp_process) ;
    if ( !q.empty() ) {
      temp_process = q[0][0] ;
      
      sort_priority(q) ;
      if(q[0][0].ID!= cpu) {
        if ( slice != 0 && output != true ) {
          for ( int j = 0 ; j < q.size() ; j++) {
            if ( temp_process.priority == q[j].front().priority) {
              q[j].push_back(temp_process) ;
              q[j].erase(q[j].begin()) ;
            } // if
          } // for
        } // if
        
        slice = timeslice ;
        cpu = q[0][0].ID;
      } // if 
      
        
      if ( q[0][0].start_time != -1)
        q[0][0].start_time = count ;
    } // if
    count++ ;
  } // while 

  return ans ;
} // PPRR()

vector<process> HRRN( vector<process> &all_process, string &gantt ) {
	int count = 0;
	int i = 0 ;
	int index = 0 ; // 記錄在all_process 的index 
	vector<process> ans ;
	vector<process> q ;
	process cpu ;
	bool idle = true ;
	while( i < all_process.size() ) { // 當process 做完時 i 才+1 
	  
    if ( index < all_process.size() ) {
      while ( all_process[index].arrival_time == count ) { // 時間到了 將process 放入 queue 
        q.push_back(all_process[index]) ;
        index++ ;
      } // while
    } // if 
    
    if ( idle ) {
      if ( count != 0 )
	      gantt = gantt + "-" ;
    } // if
	  else { // cpu execution
	    gantt = gantt + GanttChange(cpu.ID) ;
	    cpu.cpu_burst = cpu.cpu_burst - 1 ;
	    if (cpu.cpu_burst == 0) { // execution end
	      cpu.end_time = count ;
	      ans.push_back(cpu) ;
	      idle = true ;
	      i++ ;
	    } // if
    } // else
    
    if ( !q.empty() && idle ) {
      sort_ratio(q, count) ;
      cpu = q.front() ;
      q.erase(q.begin()) ;
      if (cpu.start_time != -1)
        cpu.start_time = count ;
      idle = false ;
    } // if
    count++ ;
  } // while 
  
  return ans ;
}// HRRN()

void Writefile(vector<process> all_process, string filename, int mode, string gantt ) {
  filename = "out_" + filename ;
  outfile.open(filename.c_str(), ios::out) ;
  if ( mode == 1 ) {
    outfile << "FCFS\n" ;
    outfile << gantt << '\n' << "===========================================================\n\n" << "waiting\n" ;
    outfile << "ID" << '\t' << "FCFS\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time - all_process[i].cpu  << '\n' ;
    outfile << "===========================================================\n\nTurnaround Time\n" ;
    outfile << "ID" << '\t' << "FCFS\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time << '\n' ;
  } // if
  
  else if ( mode == 2 ) {
    outfile << "RR\n" ;
    outfile << gantt << '\n' << "===========================================================\n\n" << "waiting\n" ;
    outfile << "ID" << '\t' << "RR\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
     outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time - all_process[i].cpu  << '\n' ;
    outfile << "===========================================================\n\nTurnaround Time\n" ;
    outfile << "ID" << '\t' << "RR\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time << '\n' ;
  } // else if
  
  else if ( mode == 3 ) {
    outfile << "SRTF\n" ;
    outfile << gantt << '\n' << "===========================================================\n\n" << "waiting\n" ;
    outfile << "ID" << '\t' << "SRTF\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
     outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time - all_process[i].cpu  << '\n' ;
    outfile << "===========================================================\n\nTurnaround Time\n" ;
    outfile << "ID" << '\t' << "SRTF\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time << '\n' ;
  } // else if 
  
  else if ( mode == 4 ) {
    outfile << "PPRR\n" ;
    outfile << gantt << '\n' << "===========================================================\n\n" << "waiting\n" ;
    outfile << "ID" << '\t' << "PPRR\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
     outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time - all_process[i].cpu  << '\n' ;
    outfile << "===========================================================\n\nTurnaround Time\n" ;
    outfile << "ID" << '\t' << "PPRR\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time << '\n' ;
  } // else if
  
  else if ( mode == 5 ) {
    outfile << "HRRN\n" ;
    outfile << gantt << '\n' << "===========================================================\n\n" << "waiting\n" ;
    outfile << "ID" << '\t' << "HRRN\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
     outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time - all_process[i].cpu  << '\n' ;
    outfile << "===========================================================\n\nTurnaround Time\n" ;
    outfile << "ID" << '\t' << "HRRN\n===========================================================\n" ;
    for ( int i = 0 ; i < all_process.size() ; i++ ) 
      outfile << all_process[i].ID << '\t' << all_process[i].end_time - all_process[i].arrival_time << '\n' ;
  } // else if
  
  outfile.close() ;
} // Writefile()

void WriteAll(vector< vector<process> > all_process, string filename, vector<string> gantt  ) {
  filename = "out_" + filename ;
  outfile.open(filename.c_str(), ios::out) ;
  outfile << "All\n" ;
  outfile << "==        FCFS==\n" << gantt[0] << '\n' ; 
  outfile << "==          RR==\n" << gantt[1] << '\n' ;
  outfile << "==        SRTF==\n" << gantt[2] << '\n' ;
  outfile << "==        PPRR==\n" << gantt[3] << '\n' ;
  outfile << "==        HRRN==\n" << gantt[4] << '\n' ;
  outfile << "===========================================================\n\nwaiting\n" ;
  outfile << "ID" << '\t' << "FCFS" << '\t' << "RR" << '\t' << "SRTF" << '\t' << "PPRR" << '\t' << "HRRN\n" ;
  outfile << "===========================================================\n" ;
  for ( int i = 0 ; i < all_process[0].size() ; i++ ) {
    for ( int j = 0 ; j < all_process.size() ; j++ ) {
      if ( j == 0 )
        outfile << all_process[j][i].ID  ;
      outfile << '\t' << all_process[j][i].end_time - all_process[j][i].arrival_time - all_process[j][i].cpu ;
      if ( j == 4)
        outfile << '\n' ;
    } // for
  } // for
  
  outfile << "===========================================================\n\nTurnaround Time\n" ;
  outfile << "ID" << '\t' << "FCFS" << '\t' << "RR" << '\t' << "SRTF" << '\t' << "PPRR" << '\t' << "HRRN\n" ;
  outfile << "===========================================================\n" ;
  for ( int i = 0 ; i < all_process[0].size() ; i++ ) {
    for ( int j = 0 ; j < all_process.size() ; j++ ) {
      if ( j == 0 )
        outfile << all_process[j][i].ID ;
      outfile << '\t' << all_process[j][i].end_time - all_process[j][i].arrival_time  ;
      if ( j == 4)
        outfile << '\n' ;
    } // for
  } // for
  
  outfile << "===========================================================\n\n" ;
  outfile.close() ;
} // WritteALL()

int main(){
  int mode, timeslice ;
  string filename ;
  cout << "輸入要執行的檔案 :" ;
  cin >> filename ;
  while ( !LoadFile(filename) ) { // 讀到有檔案為止 
    cout << "輸入要執行的檔案 :" ;
    cin >> filename ;
  } // while
  
  vector<process> all_process ;
  StoreFile(mode, timeslice, all_process ) ;
  if (mode == 1) {
    string gantt ;
    Sort(all_process) ;
    vector<process> ans ;
    ans = FCFS( all_process, gantt ) ;
    IDSort(ans) ;
    Writefile(ans, filename, mode, gantt) ;
  } // if
  else if ( mode == 2 ) {
    string gantt ;
    Sort(all_process) ;
    vector<process> ans ;
    ans = RR(all_process, gantt, timeslice) ;
    IDSort(ans) ;
    Writefile(ans, filename, mode, gantt) ;
  } // else if
  else if ( mode == 3 ) {
    string gantt ;
    Sort(all_process) ;
    vector<process> ans ;
    ans = SRTF(all_process, gantt) ;
    IDSort(ans) ;
    Writefile(ans, filename, mode, gantt) ;
  } // else if
  else if ( mode == 4 ) {
    string gantt ;
    Sort(all_process) ;
    vector<process> ans ;
    ans = PPRR(all_process, gantt, timeslice) ;
    IDSort(ans) ;
    Writefile(ans, filename, mode, gantt) ;
  } // else if
  else if ( mode == 5 ) {
    string gantt ;
    Sort(all_process) ;
    vector<process> ans ;
    ans = HRRN(all_process, gantt) ;
    IDSort(ans) ;
    Writefile(ans, filename, mode, gantt) ;
  } // else if
 else if ( mode == 6 ) {
   vector<string> gantt ;
   string temp_gantt ;
   Sort(all_process) ; 
   vector<process> temp_process ;
   vector< vector<process> > ans ;
   vector<process> temp_ans ; 
    // FCFS
    temp_gantt.clear() ;
    temp_process = all_process ;
    temp_ans = FCFS( temp_process, temp_gantt) ;
    IDSort(temp_ans) ;
    gantt.push_back(temp_gantt) ;
    ans.push_back(temp_ans) ;
    //RR
    temp_gantt.clear() ;
    temp_process = all_process ;
    temp_ans = RR( temp_process, temp_gantt, timeslice) ;
    IDSort(temp_ans) ;
    gantt.push_back(temp_gantt) ;
    ans.push_back(temp_ans) ;
    //SRTF
    temp_gantt.clear() ;
    temp_process = all_process ;
    temp_ans = SRTF( temp_process, temp_gantt) ;
    IDSort(temp_ans) ;
    gantt.push_back(temp_gantt) ;
    ans.push_back(temp_ans) ;
    //PPRR
    temp_gantt.clear() ;
    temp_process = all_process ;
    temp_ans = PPRR( temp_process, temp_gantt, timeslice) ;
    IDSort(temp_ans) ;
    gantt.push_back(temp_gantt) ;
    ans.push_back(temp_ans) ;
    //HRRN
    temp_gantt.clear() ;
    temp_process = all_process ;
    temp_ans = HRRN( temp_process, temp_gantt) ;
    IDSort(temp_ans) ;
    gantt.push_back(temp_gantt) ;
    ans.push_back(temp_ans) ;
    //Write
    WriteAll(ans, filename, gantt) ;
 } // else if
}// main()
