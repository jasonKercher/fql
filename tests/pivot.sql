select 
	 sum(case when bar = '00' then 1 else 0 end) [00]
	,sum(case when bar = '11' then 1 else 0 end) [11]
	,sum(case when bar = '22' then 1 else 0 end) [22]
	,sum(case when bar = '33' then 1 else 0 end) [33]
	,sum(case when bar = '44' then 1 else 0 end) [44]
	,sum(case when bar = '55' then 1 else 0 end) [55]
	,sum(case when bar = '66' then 1 else 0 end) [66]
	,sum(case when bar = '77' then 1 else 0 end) [77]
	,sum(case when bar = '88' then 1 else 0 end) [88]
	,sum(case bar when '99' then 1 else 0 end) [99]
	,sum(case when left(bar,1) != right(bar,1) then 1 else 0 end) [not_paired]
	,count(*) TOTAL
from t1
