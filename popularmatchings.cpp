#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    
    // Input the initial preference list as graph G

    vector<vector<int>> initial_list(n);

    for (int i = 0; i < n; i++) {
        int cnt; cin >> cnt;

        while (cnt--) {
            int post_pref;
            cin >> post_pref;
            post_pref--;

            initial_list[i].push_back(post_pref);
        }

        initial_list[i].push_back(-1);

    }



    // Reduce the initial graph G into G'

    vector<pair<int, int>> applicant_list(n, {-1, -1});

    unordered_set<int> f_posts;

    for (int i = 0; i < n; i++) {
        f_posts.insert(initial_list[i][0]);
        applicant_list[i].first = initial_list[i][0];
    }


    for (int i = 0; i < n; i++) {
        int ind = 0;

        while (f_posts.find(initial_list[i][ind]) != f_posts.end()) {
            ind++;
        }
        applicant_list[i].second = initial_list[i][ind];
    }



    vector<unordered_set<int>> post_graph(m);
    vector<unordered_set<int>> post_graph_copy(m);
    
    for (int i = 0; i < n; i++) {
        post_graph[applicant_list[i].first].insert(i);
        if (applicant_list[i].second != -1) post_graph[applicant_list[i].second].insert(i);
        post_graph_copy[applicant_list[i].first].insert(i);
    }




    // Match all applicants with posts of degree 1, starting with the last resorts


    unordered_set<int> matched_posts;
    

    int applicants_matched = 0;

    vector<int> M(n);


    for (int i = 0; i < n; i++) {
        if (applicant_list[i].second == -1) {
            M[i] = -1;
            applicants_matched++;
            post_graph[applicant_list[i].first].erase(i);
            applicant_list[i].first = -1;
        }
    }




    int real_index = 0;
    int current_index = 0;

    while (real_index < m) {
        if (post_graph[current_index].size() == 1) {
            int applicant = *(post_graph[current_index].begin());
            M[applicant] =  current_index;
            applicants_matched++;
            matched_posts.insert(current_index);
            int tmp_indx = current_index;
            if (applicant_list[applicant].first == current_index) tmp_indx = applicant_list[applicant].second;
            else tmp_indx = applicant_list[applicant].first;
            post_graph[applicant_list[applicant].first].erase(applicant);
            post_graph[applicant_list[applicant].second].erase(applicant);
            applicant_list[applicant] = {-1, -1};

            if (post_graph[tmp_indx].size() == 1) {
                current_index = tmp_indx;
                continue;
            }
        }
        real_index += 1;
        current_index = real_index;
    }



    


    // No need to remove 0 degree posts, if we create maximum matching by only looking at applicants

    // Comparing post count and applicant count


    int non_matched_2_degree_posts = 0;

    for (int i = 0; i < m; i++) if (post_graph[i].size() > 0) non_matched_2_degree_posts++;

    if (non_matched_2_degree_posts + matched_posts.size() < n - applicants_matched) {
        cout << "No popular matching possible... sorry" << endl;
        return 0;
    }



    // Adding maximum matching

    while (applicants_matched < n) {
        for (int i = 0; i < n; i++) {
            if (applicant_list[i].first != -1) {
                M[i] = applicant_list[i].first;
                applicants_matched++;
                matched_posts.insert(applicant_list[i].first);
                post_graph[applicant_list[i].first].erase(i);
                int other_app = *post_graph[applicant_list[i].first].begin();
                if (applicant_list[other_app].first == applicant_list[i].first) applicant_list[other_app].first = -1;
                if (applicant_list[other_app].second == applicant_list[i].first) applicant_list[other_app].second = -1;

                continue;
            }

            if (applicant_list[i].second != -1) {
                M[i] = applicant_list[i].second;
                applicants_matched++;
                matched_posts.insert(applicant_list[i].second);
                post_graph[applicant_list[i].second].erase(i);
                int other_app = *post_graph[applicant_list[i].second].begin();
                if (applicant_list[other_app].first == applicant_list[i].second) applicant_list[other_app].first = -1;
                if (applicant_list[other_app].second == applicant_list[i].second) applicant_list[other_app].second = -1;


                continue;
            }
        }
    }



    // Promoting f_posts

    for (auto f_post: f_posts) {
        if (matched_posts.find(f_post) == matched_posts.end()) M[*post_graph_copy[f_post].begin()] = f_post;
    }



    // Complete



    for (int i = 0; i < n; i++) {
        cout << i + 1 << " " << M[i] + 1 << endl;
    }


    return 0;

}