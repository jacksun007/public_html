use std::sync::{Arc, Mutex, Condvar}; 
use std::thread;
use std::time::Duration;

const N: usize = 5;

struct Chopsticks {
    /* true means available, false means used by others */
    sticks: [bool; N],
}

impl Chopsticks {
    fn can_eat(&self, eater_id: usize) -> bool {
        let left = eater_id % N;
        let right = (eater_id + 1) % N;
        self.sticks[left] && self.sticks[right]
    }
    
    fn pick_up(&mut self, eater_id: usize) {
        let left = eater_id % N;
        let right = (eater_id + 1) % N;
        self.sticks[left] = false;
        self.sticks[right] = false;
    }
    
    fn put_down(&mut self, eater_id: usize) {
        let left = eater_id % N;
        let right = (eater_id + 1) % N;
        self.sticks[left] = true;
        self.sticks[right] = true;
        /* washes the chopsticks here for next person */
    }
}

struct Monitor<T> {
    mutex: Mutex<T>,
    cvar: Condvar,
}

struct Philosopher {
    id: usize,
    monitor: Arc<Monitor<Chopsticks>>,
}

impl Philosopher {
    fn pick_up(& mut self) {
        let Monitor {mutex, cvar} = &*self.monitor; 
        let mut diner = mutex.lock().unwrap(); 
        while !diner.can_eat(self.id) {
            diner = cvar.wait(diner).unwrap();
        }
        diner.pick_up(self.id);
    }

    fn put_down(& mut self) {
        let Monitor {mutex, cvar} = &*self.monitor; 
        let mut diner = mutex.lock().unwrap(); 
        diner.put_down(self.id);
        cvar.notify_all();
    }
    
    fn think(&self) {
        println!("Philosopher {} is thinking.", self.id);
        thread::sleep(Duration::from_micros(1));
    }
    
    fn eat(&self) {
        println!("Philosopher {} is eating.", self.id);
        thread::sleep(Duration::from_micros(1));
    }
}

fn main() {
    let mut threads = vec![];
    let monitor = Arc::new(Monitor {
        mutex: Mutex::new(Chopsticks { sticks: [true; N], }), 
        cvar: Condvar::new(), 
    });   

    const REPEAT: i32 = 5;

    for i in 1..=N {
        let monitor = monitor.clone();
        threads.push(thread::spawn(move || {
            let mut philosopher = Philosopher {
                id: i,
                monitor: monitor,
            };
            
            for _ in 0..REPEAT {
                /* philosopher is hungry */
	            philosopher.pick_up();
                philosopher.eat();
                philosopher.put_down();
                philosopher.think();
	        }
	    }));
	}
	
	for child in threads {
	    child.join().unwrap();
	}
}
