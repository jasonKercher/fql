select idx, fifo
	,case when rootidx = -1 then 'NONE' else rootidx end rootidx
	,min(fifo_avail+0) min_avail
	,max(fifo_avail+0) max_avail
	,avg(fifo_avail+0.0) avg_avail
	,min(fifo_recv+0) min_recv
	,max(fifo_recv+0) max_recv
	,avg(fifo_recv+0.0) avg_recv
from _debug
group by idx, fifo, rootidx
order by idx, fifo
