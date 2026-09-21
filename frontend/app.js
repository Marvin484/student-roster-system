const API_BASE = "http://localhost:8080";

const rosterBody = document.getElementById("roster-body");
const tableMessage = document.getElementById("table-message");
const formMessage = document.getElementById("form-message");
const invalidEmailsList = document.getElementById("invalid-emails-list");
const filterSelect = document.getElementById("filter-select");
const addForm = document.getElementById("add-form");

async function fetchStudents(filter = "ALL") {
  tableMessage.textContent = "";
  try {
    const url = filter === "ALL"
      ? `${API_BASE}/students`
      : `${API_BASE}/students/degree/${filter}`;
    const res = await fetch(url);
    if (!res.ok) throw new Error(`Server responded with ${res.status}`);
    const students = await res.json();
    renderTable(students);
  } catch (err) {
    tableMessage.textContent = `Could not load roster: ${err.message}. Is roster_server.exe running?`;
  }
}

async function fetchInvalidEmails() {
  try {
    const res = await fetch(`${API_BASE}/students/invalid-emails`);
    if (!res.ok) throw new Error(`Server responded with ${res.status}`);
    const emails = await res.json();
    invalidEmailsList.innerHTML = "";
    if (emails.length === 0) {
      invalidEmailsList.innerHTML = "<li>None found.</li>";
      return;
    }
    emails.forEach(email => {
      const li = document.createElement("li");
      li.textContent = email;
      invalidEmailsList.appendChild(li);
    });
  } catch (err) {
    invalidEmailsList.innerHTML = `<li>Could not load: ${err.message}</li>`;
  }
}

function average(days) {
  return ((days[0] + days[1] + days[2]) / 3).toFixed(4);
}

function renderTable(students) {
  rosterBody.innerHTML = "";
  if (students.length === 0) {
    tableMessage.textContent = "No students found.";
    return;
  }
  students.forEach(s => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${s.studentId}</td>
      <td>${s.firstName}</td>
      <td>${s.lastName}</td>
      <td>${s.email}</td>
      <td>${s.age}</td>
      <td>{${s.daysInCourse.join(", ")}}</td>
      <td>${average(s.daysInCourse)}</td>
      <td>${s.degreeProgram}</td>
      <td><button class="danger" data-id="${s.studentId}">Delete</button></td>
    `;
    rosterBody.appendChild(row);
  });

  rosterBody.querySelectorAll("button.danger").forEach(btn => {
    btn.addEventListener("click", () => deleteStudent(btn.dataset.id));
  });
}

async function deleteStudent(id) {
  if (!confirm(`Remove student ${id}?`)) return;
  try {
    const res = await fetch(`${API_BASE}/students/${id}`, { method: "DELETE" });
    if (res.status === 404) {
      tableMessage.textContent = `Student ${id} was not found.`;
      return;
    }
    if (!res.ok) throw new Error(`Server responded with ${res.status}`);
    refreshAll();
  } catch (err) {
    tableMessage.textContent = `Delete failed: ${err.message}`;
  }
}

addForm.addEventListener("submit", async (e) => {
  e.preventDefault();
  formMessage.className = "message";
  formMessage.textContent = "";

  const payload = {
    studentId: document.getElementById("studentId").value.trim(),
    firstName: document.getElementById("firstName").value.trim(),
    lastName: document.getElementById("lastName").value.trim(),
    email: document.getElementById("email").value.trim(),
    age: parseInt(document.getElementById("age").value, 10),
    degreeProgram: document.getElementById("degreeProgram").value,
    daysInCourse: [
      parseInt(document.getElementById("days1").value, 10),
      parseInt(document.getElementById("days2").value, 10),
      parseInt(document.getElementById("days3").value, 10)
    ]
  };

  try {
    const res = await fetch(`${API_BASE}/students`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || `Server responded with ${res.status}`);

    formMessage.className = "message success";
    formMessage.textContent = `Added student ${data.studentId}.`;
    addForm.reset();
    refreshAll();
  } catch (err) {
    formMessage.textContent = err.message;
  }
});

filterSelect.addEventListener("change", () => fetchStudents(filterSelect.value));
document.getElementById("refresh-btn").addEventListener("click", () => refreshAll());

function refreshAll() {
  fetchStudents(filterSelect.value);
  fetchInvalidEmails();
}

refreshAll();
