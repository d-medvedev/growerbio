import requests
import matplotlib.pyplot as plt
import pdfkit

# получение токена
token = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiI2NDIxYWVlM2Q0YjBmODAwMTA3MTI1NDIiLCJzdWIiOiI2MmYzOTBiZmJiMGVlMDAwMTEyYjkyZmIiLCJncnAiOiI2M2YzYTcwY2NhYjQ3ZTAwMTBlYWM5MjYiLCJvcmciOiI2M2YzYTcwY2NhYjQ3ZTAwMTBlYWM5MjYiLCJsaWMiOiI1ZDNiNWZmMDBhMGE3ZjMwYjY5NWFmZTMiLCJ1c2ciOiJhcGkiLCJmdWxsIjpmYWxzZSwicmlnaHRzIjoxLjUsImlhdCI6MTY3OTkyOTA1OSwiZXhwIjoxNjgyNDQyMDAwfQ.dZKsVqZLLZEnLN8P_nNJFdbvrRzo-lfVEmk2Zv1b55U'

# формирование URL-адреса для запроса
url = 'http://example.com/data?token='

# отправка GET-запроса и получение ответа в формате JSON
# отправляем http запрос с токеном
response = requests.get(url, headers={'Authorization': f'Bearer {token}'})
data = response.json()