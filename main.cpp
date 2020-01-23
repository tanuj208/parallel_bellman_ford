/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

#define pb push_back
#define send_data_tag 2001
#define sz(a) (ll)(a.size())
#define mp make_pair
#define ff first
#define ss second
#define pii pair<ll,ll>
#define INF 1e18

int main( int argc, char **argv ) {
	int rank, numprocs;
	int root_process = 0;
	int ierr;
	MPI_Status status;
	ll i;
    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    if(rank==0)
    {
    	ll n,m;
    	cin>>n>>m;
    	vector<ll> ad[n+5];
    	vector<ll> wt_ad[n+5];
    	vector<ll> vtx_list;
    	ll i;
    	for(i=0;i<m;i++)
    	{
    		ll u,v,w;
    		cin>>u>>v>>w;
    		ad[u].pb(v);
    		wt_ad[u].pb(w);
    		ad[v].pb(u);
    		wt_ad[v].pb(w);
    	}
    	ll s;
    	cin>>s;

	    ierr = MPI_Bcast(&n, 1, MPI_LONG, root_process, MPI_COMM_WORLD); // total number of vertices    	

    	for(i=0;i<=n;i++)
    		vtx_list.pb(i);
    	ll vtx_len = n/numprocs;
	    for(ll pid = 1;pid < numprocs;pid++)
	    {
	    	ll start_idx = 1 + vtx_len*(pid-1);
		    ll numbers_cnt;
	    	if(pid == numprocs-1)
	    		numbers_cnt = 1 + n - start_idx;
	    	else
		    	numbers_cnt = 1 + vtx_len*pid - start_idx;

		    ierr = MPI_Send(&numbers_cnt, 1, MPI_LONG, pid, send_data_tag, MPI_COMM_WORLD); // number of vertices for each process
		    ierr = MPI_Send(&vtx_list[start_idx], numbers_cnt, MPI_LONG, pid, send_data_tag, MPI_COMM_WORLD); // list of processes
		    for(i=start_idx;i<start_idx+numbers_cnt;i++)
		    {
		    	ll ad_sz = sz(ad[i]);
			    ierr = MPI_Send(&ad_sz, 1, MPI_LONG, pid, send_data_tag, MPI_COMM_WORLD); // size of adjacency list of one vtx
		    	ierr = MPI_Send(&ad[i][0], sz(ad[i]), MPI_LONG, pid, send_data_tag, MPI_COMM_WORLD); // adjacency list of one vtx
		    	ierr = MPI_Send(&wt_ad[i][0], sz(wt_ad[i]), MPI_LONG, pid, send_data_tag, MPI_COMM_WORLD); // weight list of one vtx
		    }
	    }

	    vector<ll> wt_arr(n, INF);
	    wt_arr[s-1]=0;

	    for(i=0;i<n;i++)
	    {
		    ierr = MPI_Bcast(&wt_arr[0], n, MPI_LONG, root_process, MPI_COMM_WORLD);
		    ierr = MPI_Reduce(MPI_IN_PLACE, &wt_arr[0], n, MPI_LONG, MPI_MIN, root_process, MPI_COMM_WORLD);
	    }
	    for(i=0;i<n;i++)
	    	cout<<i+1<<" "<<wt_arr[i]<<endl;
	}
    else
    {
    	ll n;
	    ierr = MPI_Bcast(&n, 1, MPI_LONG, root_process, MPI_COMM_WORLD); 	
    	ll vtx_len;
    	ierr = MPI_Recv(&vtx_len, 1, MPI_LONG, root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    	vector<ll> vtx_list(vtx_len);
    	ierr = MPI_Recv(&vtx_list[0], vtx_len, MPI_LONG, root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    	vector<vector<ll>> local_ad;
    	vector<vector<ll>> wt_local_ad;

    	ll i;
    	for(i=0;i<vtx_len;i++)
    	{
    		ll ad_sz;
	    	ierr = MPI_Recv(&ad_sz, 1, MPI_LONG, root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    		vector<ll> tmp(ad_sz);
    		vector<ll> tmp2(ad_sz);
	    	ierr = MPI_Recv(&tmp[0], ad_sz, MPI_LONG, root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	    	ierr = MPI_Recv(&tmp2[0], ad_sz, MPI_LONG, root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	    	local_ad.pb(tmp);
	    	wt_local_ad.pb(tmp2);
    	}

    	vector<ll> wt_arr(n);

    	for(i=0;i<n;i++)
    	{
		    ierr = MPI_Bcast(&wt_arr[0], n, MPI_LONG, root_process, MPI_COMM_WORLD);
		    ll j;

		    for(j=0;j<sz(vtx_list);j++)
		    {
		    	ll k;
		    	ll vtx = vtx_list[j]-1;
		    	for(k=0;k<sz(local_ad[j]);k++)
		    	{
		    		ll oth_vtx = local_ad[j][k]-1;
		    		ll wt = wt_local_ad[j][k];

		    		if(wt_arr[oth_vtx] > wt_arr[vtx] + wt)
		    			wt_arr[oth_vtx] = wt_arr[vtx] + wt;
		    	}
		    }
		    ierr = MPI_Reduce(&wt_arr[0], &wt_arr[0], n, MPI_LONG, MPI_MIN, root_process, MPI_COMM_WORLD);
    	}
    }


    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}