def recv_count():
	try:
		while True:
			n = yield
			print(n, end=" ")
	except GeneratorExit:	# triggers upon close()
		print("Kaboom!")
		
r = recv_count()
r.send(None)		# starts the function
for i in range(5,0,-1):
	r.send(i)
r.close()			# closes the generator object

def coroutine(func):
    def start(*args,**kwargs):
        cr = func(*args,**kwargs)
        cr.send(None)
        return cr
    return start

# using decorator the manual way    
recv_count = coroutine(recv_count)

# don't have to prime it anymore!
r = recv_count()
for i in range(5, 0, -1):
	r.send(i)
r.close()	

def counter(maximum):
    i = 0
    while i < maximum:
        val = (yield i)
        # If value provided, change counter
        if val is not None:
            i = val
        else:
            i += 1
            
it = counter(10)
print(next(it), next(it)) 
print(it.send(8))
print(next(it))
print(next(it))
