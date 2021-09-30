select idx, fifo
	,case when rootidx = -1 then 'NONE' else rootidx end rootidx
	,min(fifo_avail+0) minavail
	,max(fifo_avail+0) maxavail
	,min(fifo_recv+0) minrecv
	,max(fifo_recv+0) minrecv
from _debug
group by idx, fifo, rootidx
order by idx, fifo
