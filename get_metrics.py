from os import listdir
from os.path import isfile, join

import pandas as pd
import plotly.graph_objects as go

files = [f for f in listdir("./outputs/greedy") if isfile(join("./outputs/greedy", f))]

num_movies = {}
screen_time = {}
execution_duration = {}

for file_name in files:
    num_movies[file_name] = {}
    screen_time[file_name] = {}
    execution_duration[file_name] = {}
    with open('./outputs/greedy/{}'.format(file_name), 'r') as f:
        lines = f.readlines()
        num_movies[file_name]['greedy'] = int(float(lines[-3].split()[-1]))
        screen_time[file_name]['greedy'] = int(float(lines[-2].split()[-1]))
        execution_duration[file_name]['greedy'] = int(float(lines[-1].split()[-1]))
        
        

    with open('./outputs/random/{}'.format(file_name), 'r') as f:
        lines = f.readlines()
        num_movies[file_name]['random'] = int(float(lines[-3].split()[-1]))
        screen_time[file_name]['random'] = int(float(lines[-2].split()[-1]))
        execution_duration[file_name]['random'] = int(float(lines[-1].split()[-1]))

num_movies_df = pd.DataFrame.from_dict(num_movies, 'index')
screen_time_df = pd.DataFrame.from_dict(screen_time, 'index')
execution_duration_df = pd.DataFrame.from_dict(execution_duration, 'index')

fig = go.Figure()
fig.add_trace(go.Bar(
    x=files,
    y=num_movies_df['greedy'],
    name='Greedy Algorithm',
    marker_color='indianred'
))
fig.add_trace(go.Bar(
    x=files,
    y=num_movies_df['random'],
    name='Stochastic Algorithm',
    marker_color='lightsalmon'
))

fig.update_layout(barmode='group', xaxis_tickangle=-45)
fig.show()