#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32
uint32_t expr(char *e, bool *success);
static WP wp_pool[NR_WP];
static WP *head, *free_;

WP* new_wp();
void free_wp(int n);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
 /* for(int i=0;i<NR_WP;i++)
  {
	  wp_pool[i]->prev=wp_pool[i-1];
  }
  wp_pool[0]->prev=NULL;*/
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp()
{
	if(free_==NULL)
	{
		printf("There is no avalialbe watchpoint\n");
		assert(0);
 	}
    WP *x;
	x=free_;
	free_=free_->next;
	x->next=head;
	head=x;
	return head;
}

void free_wp(int n)
{
	WP *x;
	x=head;
	WP *y=x;
	if(head==NULL)
	{
		printf("There is no watchpoint\n");
		/*assert(0);*/
		return;
	} 
	while((x->NO!=n)&&x->next!=NULL)
	{
		y=x;
		x=x->next;
	} 
	if(x->NO==n)
	{ 
		if(x==head) head=head->next;
		else
	 	{
			y->next=x->next;
			x->next=free_;
			free_=x;
		}
	}
};

void pwatchpoint()
{
	WP *wp;
	wp = head;
/*	bool success = true;*/
	if(wp==NULL){
		printf("There is not such watchpoint.\n");
		return;
	}
	while(wp!=NULL)
	{
		/*uint32_t tmp=expr(wp->bufs, &success);*/
		printf("WATCHPOINT NO.%d  %s: %u\n",wp->NO,wp->bufs,wp->ans);
		wp=wp->next;
	}
}

bool checkchange()
{
	bool flag = true;
	bool success = true;
	WP *wp=head;
	while(wp!=NULL)
	{
		uint32_t tmp=expr(wp->bufs,&success);
		if(tmp!=wp->ans)
		{
			printf("Watchpoint %d has changed,the old value is %u and now is %u\n",wp->NO,wp->ans,tmp);
			flag=false;
			wp->ans=tmp;
		}
		wp=wp->next;
	}
	return flag;
}
