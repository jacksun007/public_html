use std::sync::{Arc, Mutex, Condvar}; 
use std::thread;
use std::time::Duration;

const MAXLEN: usize = 8;

struct Bounded {
    buffer: [i32; MAXLEN],
    top: usize,
    bottom: usize,
}

impl Bounded {
    fn push(& mut self, val: i32) {
        self.buffer[self.top] = val;
	    self.top = (self.top + 1) % MAXLEN;
    }
    
    fn pop(& mut self) -> i32 {
        let val = self.buffer[self.bottom];
	    self.bottom = (self.bottom + 1) % MAXLEN;
	    val
    }
    
    fn is_empty(& self) -> bool {
        self.bottom == self.top
    }
    
    fn is_full(& self) -> bool {
        (self.bottom + 1) % MAXLEN == self.top
    }
}

struct Monitor<T> {
    mutex: Mutex<T>,
    empty: Condvar,
    full: Condvar,
}

fn main() {
    let mut threads = vec![];
    let monitor = Arc::new(Monitor {
        mutex: Mutex::new(Bounded { buffer: [0; MAXLEN], top: 0, bottom: 0 }), 
        empty: Condvar::new(), 
        full: Condvar::new(),
    });   
    
    const NPRODUCER: i32 = 3;
    const NPRODUCT: i32 = 10;
    const NCONSUMER: i32 = 2;
    const NCONSUMED: usize = (NPRODUCER * NPRODUCT / NCONSUMER) as usize;
    
    for i in 1..=NPRODUCER {
        let monitor = monitor.clone();
        threads.push(thread::spawn(move || { 
            for j in 0..NPRODUCT {
                let val = i * 10 + j;
                thread::sleep(Duration::from_micros(1));
                
	            let Monitor {mutex, empty, full} = &*monitor; 
	            let mut circ = mutex.lock().unwrap(); 
	            while circ.is_full() {
	                circ = full.wait(circ).unwrap();
	            }
	            circ.push(val);
	            empty.notify_all();
	        }
	    }));
	}
	
	for i in 1..=NCONSUMER {
        let monitor = monitor.clone();
        threads.push(thread::spawn(move || { 
            let mut v = vec![];
            
            while v.len() < NCONSUMED {
	            let Monitor {mutex, empty, full} = &*monitor; 
	            let mut circ = mutex.lock().unwrap(); 
	            while circ.is_empty() {
	                circ = empty.wait(circ).unwrap();
	            }
	            v.push(circ.pop());
	            full.notify_all();
	        }
	        
	        println!("thread {} consumed: {:?}", i, v);
	    }));
	}
	
	for child in threads {
	    child.join().unwrap();
	}
}
