select idx, fifo
	,case when rootidx = -1 then 'NONE' else rootidx end rootidx
	,min(avail+0) min_av
	,max(avail+0) max_av
	,avg(avail+0.0) avg_avail /* lol for spacing */
	,min(receiv+0) min_rec
	,max(receiv+0) max_rec
	,avg(receiv+0.0) avg_rec
from _debug
where receiv != ''
group by idx, fifo, rootidx
order by idx, fifo
