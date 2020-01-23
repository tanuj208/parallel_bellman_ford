#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> p;

#define pb push_back
#define mp make_pair
#define sz(a) (ll)(a).size()
#define INF 1e18

vector<p> ad[10004];
ll A,B;
vector<ll> d(10004,INF);
priority_queue <p,vector<p>,greater<p> > pp;
// p means pair
// this priority_queue ==> min heap

vector<ll> visited(10005,0);
// array to make sure that any vertex should be iterated more than once

void dijkstra()
{
	// till priority queue is not empty
	while(!pp.empty())
	{
		ll i;
		ll v=(pp.top()).second;
		// take the vertex(v) with minimum d[v]
		if(visited[v]==1)
		{
			pp.pop();
			continue;
		}
		visited[v]=1;
		ll wt=(pp.top()).first;
		// take the weight of v (also equal to d[v])
		pp.pop();
		for(i=0;i<sz(ad[v]);i++)
		{
			// iterate through each of its child and if d[x](x being child) is more than d[v]+weight of edge then update and put it in queue
			ll totalwt=wt+ad[v][i].second;
			if(totalwt<d[ad[v][i].first])
			{
				d[ad[v][i].first]=totalwt;
				pp.push(mp(d[ad[v][i].first],ad[v][i].first));
			}
		}
	}
	return;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	ll v,k;
	cin>>v>>k;
	ll i;
	for(i=0;i<k;i++)
	{
		ll a,b,c;
		cin>>a>>b>>c;
		ad[a].pb(mp(b,c));
		ad[b].pb(mp(a,c));
	}
	cin>>A;
	d[A]=0;
	pp.push(mp(0,A));
	dijkstra();
	for(i=1;i<=v;i++)
		cout<<i<<" "<<d[i]<<endl;
	return 0;
}