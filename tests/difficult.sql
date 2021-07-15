
select top (1*3)
     (select top 1 foo from t1),
     bar+'eex',
     max(baz+0),
     qty
from (
     select bar shnt, count(*) qty
     from t3
     group by bar
) x1
join t3 x2
    on  x2.bar = x1.shnt
    and 1=1
where len(bar) = right('shnt2',1)
group by bar,qty
having min(baz+0) < 20000
order by bar desc, 3

