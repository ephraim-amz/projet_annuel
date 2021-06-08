# Utilisaons de Dask au lieu de pandas afin d'avoir de meilleures performances concernant les gros datasets
import dask.dataframe as dd
import urllib
import re
from bs4 import BeautifulSoup

type_dic = {
    'tconst': 'object',
    'startYear': 'object',
    'isAdult': 'object',
    'primaryTitle': 'object',
    'titleType': 'object',
    'originalTitle': 'object',
    'endYear': 'object',
    'runtimeMinutes': 'object',
    'genres': 'object'
}

df_movies = dd.read_csv('./data/title.basics.tsv/data.txt', sep='|', error_bad_lines=False, dtype=type_dic).compute()
df_movies = df_movies[(df_movies['titleType'].eq(
    'tvMovie') | df_movies['titleType'].eq('movie')) & ~df_movies['isAdult'].eq('1')]

df_ratings = dd.read_csv('./data/title.ratings.tsv/data.txt',
                         error_bad_lines=False).compute()

df_movies_ratings = dd.merge(df_movies, df_ratings, how='left', on='tconst')
df_movies_ratings['PrimaryName'] = None
df_movies_ratings['AIRating'] = None

primary_name_column = df_movies_ratings.pop('PrimaryName')
df_movies_ratings.insert(4, 'PrimaryName', primary_name_column)


def get_primary_name(tconst):
    url = "https://www.imdb.com/title/" + tconst + "/"
    page = urllib.request.urlopen(url,timeout=10)
    soup = BeautifulSoup(page, 'html.parser')
    results = soup.find_all('a', attrs={'data-testid':"title-cast-item__actor"})
    first_result = None
    primary_name = None
    for i,result in enumerate(results):
        if i == 0:
            first_result = result.text
            m = re.search(r">(.*?)<", first_result,flags=re.MULTILINE)
            if not isinstance(m, type(None)):
                primary_name = m.group(0).replace('>', '').replace('<', '')
    return primary_name

df_movies_ratings['PrimaryName'] = df_movies_ratings.apply(lambda x: get_primary_name(x['tconst']), axis=1)
df_movies_ratings.to_csv('movie_ratings_dataset_BIS.csv', index=None)

# df_actors = dd.read_csv('./data/name.basics.tsv/data.txt', error_bad_lines=False, warn_bad_lines=True).compute()
# df_credits = dd.read_csv('./data/title.principals.tsv/data.txt', sep='|', error_bad_lines=False, warn_bad_lines=True).compute()
