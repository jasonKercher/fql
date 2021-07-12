select t1.bar, t2.bar, x.bar             
from t1                                  
join t2                                  
    on t1.baz = t2.baz                   
join t2 X                                
    on left(t1.foo, 2) = right(x.foo, 2) 

